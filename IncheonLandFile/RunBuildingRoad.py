import os
import shutil
import time
from concurrent.futures import ThreadPoolExecutor
from korean_romanizer.romanizer import Romanizer

# [공통 설정 항목]
ROOT_DIR = r"D:\KoreaLandInfo\Nanum"
SHP_EXTENSIONS = [".shp", ".shx", ".dbf", ".prj", ".qpj", ".cpg"]

# [각 데이터별 대상 파일 및 출력 폴더 지정]
BUILDING_TARGETS = ["N3A_B0010000"]
BUILDING_OUTPUT_DIR = os.path.join(ROOT_DIR, "BuildingSHP")

ROAD_TARGETS = ["N3L_A0020000", "N3L_A0010000", "N3A_A0033320"]
ROAD_OUTPUT_DIR = os.path.join(ROOT_DIR, "RoadSHP")

def clean_and_romanize(text):
    romanized = Romanizer(text).romanize()
    return romanized.replace(" ", "_").lower()

def copy_file_task(src, dst):
    """백그라운드에서 조용히 복사를 수행"""
    try:
        shutil.copy2(src, dst)
    except Exception as e:
        print(f"\n❌ 복사 실패: {src} -> {e}")

def process_gis_data(data_type, target_files, output_dir):
    """건물 또는 도로 데이터를 스캔하고 고속 복사하는 공통 함수"""
    if not os.path.exists(ROOT_DIR):
        print(f"오류: {ROOT_DIR} 경로가 존재하지 않습니다.")
        return

    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
        print(f"📂 통합 {data_type} 폴더 생성 완료: {output_dir}")

    # 최대 16개 스레드로 병렬 처리
    executor = ThreadPoolExecutor(max_workers=16)
    
    print(f"🚀 [{data_type} 데이터] 실시간 스캔 및 고속 복사를 시작합니다...")
    print("--------------------------------------------------")

    sub_folders = [f for f in os.listdir(ROOT_DIR) if os.path.isdir(os.path.join(ROOT_DIR, f)) and f.startswith("2MAP5000_SHP_")]
    total_folders = len(sub_folders)

    for idx, folder_name in enumerate(sub_folders, 1):
        ko_region = folder_name.replace("2MAP5000_SHP_","")
        en_region = clean_and_romanize(ko_region)
        
        parent_folder_path = os.path.join(ROOT_DIR, folder_name)
        
        # 현재 어느 폴더를 스캔 중인지 실시간 출력
        print(f"📦 [{idx}/{total_folders}] ({data_type}) 작업 지역: {ko_region} ({en_region})")
        
        for root, dirs, files in os.walk(parent_folder_path):
            # 생성된 결과물 폴더들은 스캔에서 제외하여 무한루프 방지
            if any(x in root for x in ["SHP_도로_추출", "SHP_건물_추출", "BuildingSHP", "RoadSHP"]):
                continue
                
            for file in files:
                file_name, file_ext = os.path.splitext(file)
                if file_name in target_files and file_ext.lower() in SHP_EXTENSIONS:
                    # 데이터 타입(Building / Road)에 맞는 파일명 조립
                    new_file_name = f"{en_region}_{data_type}_{file_name}{file_ext}"
                    src_file_path = os.path.join(root, file)
                    dst_file_path = os.path.join(output_dir, new_file_name)
                    
                    # 발견 즉시 백그라운드 복사 스레드 실행
                    executor.submit(copy_file_task, src_file_path, dst_file_path)

    print("\n--------------------------------------------------")
    print(f"⏳ 모든 {data_type} 복사 작업을 마무리하는 중...")
    executor.shutdown(wait=True)
    print(f"✅ {data_type} 데이터 통합 완료!\n")

if __name__ == "__main__":
    total_start_time = time.time()
    
    print("=== 🛠️ 전국의 GIS 데이터(건물/도로) 통합 추출 프로그램을 시작합니다 ===\n")
    
    # 1. 건물 데이터 먼저 추출
    start_b = time.time()
    process_gis_data("Building", BUILDING_TARGETS, BUILDING_OUTPUT_DIR)
    print(f"⏱️ 건물 데이터 소요 시간: {time.time() - start_b:.2f}초\n")
    
    print("==================================================\n")
    
    # 2. 이어서 도로 데이터 추출
    start_r = time.time()
    process_gis_data("Road", ROAD_TARGETS, ROAD_OUTPUT_DIR)
    print(f"⏱️ 도로 데이터 소요 시간: {time.time() - start_r:.2f}초\n")
    
    print("==================================================")
    print(f"🎉 모든 작업이 완벽하게 완료되었습니다!")
    print(f"⏱️ 총 누적 소요 시간: {time.time() - total_start_time:.2f}초")