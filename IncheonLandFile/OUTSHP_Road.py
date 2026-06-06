import os
import shutil
# 한글을 로마자(영어)로 자동 변환해 주는 라이브러리
# pip install korean-romanizer 해야함
from korean_romanizer.romanizer import Romanizer

# 1. 설정 항목
ROOT_DIR = "./" 
TARGET_FILES = ["N3L_A0020000", "N3L_A0010000", "N3A_A0033320"]
SHP_EXTENSIONS = [".shp", ".shx", ".dbf", ".prj", ".qpj", ".cpg"]

def clean_and_romanize(text):
    """
    한글 지역명을 영어로 변환하고 빈칸이나 특수문자를 보기 좋게 정리하는 함수
    예: '경기_용인시_수지구' -> 'gyeonggi_yonginsi_sujigu'
    """
    # korean_romanizer는 문자열 전체를 한 번에 변환합니다.
    romanized = Romanizer(text).romanize()
    
    # 혹시 모를 공백 제거 및 소문자 변환으로 파일명 깔끔하게 정리
    return romanized.replace(" ", "_").lower()

def extract_and_rename_shp_auto():
    if not os.path.exists(ROOT_DIR):
        print(f"오류: {ROOT_DIR} 경로가 존재하지 않습니다.")
        return

    for folder_name in os.listdir(ROOT_DIR):
        # 2. '2MAP5000_SHP_'로 시작하는 부모 지역 폴더 필터링
        if os.path.isdir(os.path.join(ROOT_DIR, folder_name)) and folder_name.startswith("2MAP5000_SHP_"):
            
            # 폴더명에서 실제 한글 지역명만 분리 (예: 경기_용인시_수지구)
            ko_region = folder_name.replace("2MAP5000_SHP_", "")
            
            # 🔥 [자동화 핵심] 한글 지역명을 자동으로 영어로 변환
            en_region = clean_and_romanize(ko_region)
            
            parent_folder_path = os.path.join(ROOT_DIR, folder_name)
            output_dir = os.path.join(parent_folder_path, "SHP_추출")
            
            # 3. 하위 폴더 탐색
            for root, dirs, files in os.walk(parent_folder_path):
                if "SHP_추출" in root:
                    continue
                    
                for file in files:
                    file_name, file_ext = os.path.splitext(file)
                    file_ext_lower = file_ext.lower()
                    
                    # 4. 원하는 파일 이름이고, SHP 관련 확장자인지 확인
                    if file_name in TARGET_FILES and file_ext_lower in SHP_EXTENSIONS:
                        
                        if not os.path.exists(output_dir):
                            os.makedirs(output_dir)
                            print(f"\n📂 새 폴더 생성: {output_dir} (지역 영문명: {en_region})")
                        
                        # 새로운 파일명 조립 (예: gyeonggi_yonginsi_sujigu_N3L_A0020000.shp)
                        new_file_name = f"{en_region}_{file_name}{file_ext}"
                        
                        src_file_path = os.path.join(root, file)
                        dst_file_path = os.path.join(output_dir, new_file_name)
                        
                        shutil.copy2(src_file_path, dst_file_path)
                        print(f"   ↳ [복사 완료] {file} -> {new_file_name}")

if __name__ == "__main__":
    print("🚀 [자동 매핑 버전] SHP 파일 추출 및 이름 변경을 시작합니다.")
    extract_and_rename_shp_auto()
    print("\n✅ 모든 자동화 작업이 완료되었습니다.")