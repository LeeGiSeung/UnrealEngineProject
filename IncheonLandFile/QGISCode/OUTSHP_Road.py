import os
import shutil
from korean_romanizer.romanizer import Romanizer

# 설정 항목
ROOT_DIR = r"D:\KoreaLandInfo\Nanum" 
INTEGRATED_OUTPUT_DIR = os.path.join(ROOT_DIR, "RoadSHP")
TARGET_FILES = ["N3L_A0020000", "N3L_A0010000", "N3A_A0033320"] # 도로 대상 파일
SHP_EXTENSIONS = [".shp", ".shx", ".dbf", ".prj", ".qpj", ".cpg"]

def clean_and_romanize(text):
    romanized = Romanizer(text).romanize()
    return romanized.replace(" ", "_").lower()

def extract_road_shp():
    if not os.path.exists(ROOT_DIR):
        print(f"오류: {ROOT_DIR} 경로가 존재하지 않습니다.")
        return

    if not os.path.exists(INTEGRATED_OUTPUT_DIR):
        os.makedirs(INTEGRATED_OUTPUT_DIR)
        print(f"📂 통합 도로 폴더 생성 완료: {INTEGRATED_OUTPUT_DIR}\n")
    
    print("🚀 [도로 데이터] 하위 폴더명 명시형 순차 복사를 시작합니다...")
    print("--------------------------------------------------")

    sub_folders = [f for f in os.listdir(ROOT_DIR) if os.path.isdir(os.path.join(ROOT_DIR, f)) and f.startswith("2MAP5000_SHP_")]
    total_folders = len(sub_folders)

    for idx, folder_name in enumerate(sub_folders, 1):
        ko_region = folder_name.replace("2MAP5000_SHP_", "")
        en_region = clean_and_romanize(ko_region)
        
        parent_folder_path = os.path.join(ROOT_DIR, folder_name)
        print(f"\n📂 [{idx}/{total_folders}] 현재 확인 중인 지역: {ko_region} ({en_region})")
        
        for root, dirs, files in os.walk(parent_folder_path):
            if "SHP_도로_추출" in root or "SHP_건물_추출" in root or "RoadSHP" in root or "BuildingSHP" in root:
                continue
                
            # 현재 탐색 중인 최하위 폴더의 이름을 가져옵니다 (예: 20141, 24132 등)
            current_subfolder_name = os.path.basename(root)
            
            for file in files:
                file_name, file_ext = os.path.splitext(file)
                if file_name in TARGET_FILES and file_ext.lower() in SHP_EXTENSIONS:
                    
                    # ★ 중복 방지를 위해 영문지역명 + 숫자폴더명 + Road 형식을 조립합니다 ★
                    new_file_name = f"{en_region}_{current_subfolder_name}_Road_{file_name}{file_ext}"
                    
                    src_file_path = os.path.join(root, file)
                    dst_file_path = os.path.join(INTEGRATED_OUTPUT_DIR, new_file_name)
                    
                    try:
                        shutil.copy2(src_file_path, dst_file_path)
                    except Exception as e:
                        print(f"   ↳ ❌ 복사 실패! ({file}): {e}")

if __name__ == "__main__":
    import time
    start_time = time.time()
    extract_road_shp()
    print("\n--------------------------------------------------")
    print(f"✅ 모든 도로 데이터가 중복 없이 RoadSHP 폴더로 모였습니다!")
    print(f"⏱️ 총 소요 시간: {time.time() - start_time:.2f}초")