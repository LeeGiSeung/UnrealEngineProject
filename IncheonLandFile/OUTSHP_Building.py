import os
import shutil

# pip install korean-romanizer 해야함

from korean_romanizer.romanizer import Romanizer

# 설정 항목
ROOT_DIR = "./" 
TARGET_FILES = ["N3L_B0020000", "N3A_B0010000"] # 건물 대상 파일
SHP_EXTENSIONS = [".shp", ".shx", ".dbf", ".prj", ".qpj", ".cpg"]

def clean_and_romanize(text):
    romanized = Romanizer(text).romanize()
    return romanized.replace(" ", "_").lower()

def extract_building_shp():
    if not os.path.exists(ROOT_DIR):
        print(f"오류: {ROOT_DIR} 경로가 존재하지 않습니다.")
        return

    for folder_name in os.listdir(ROOT_DIR):
        if os.path.isdir(os.path.join(ROOT_DIR, folder_name)) and folder_name.startswith("2MAP5000_SHP_"):
            
            ko_region = folder_name.replace("2MAP5000_SHP_", "")
            en_region = clean_and_romanize(ko_region)
            
            parent_folder_path = os.path.join(ROOT_DIR, folder_name)
            # 건물 데이터는 'SHP_건물_추출' 폴더에 따로 모읍니다.
            output_dir = os.path.join(parent_folder_path, "SHP_건물_추출")
            
            for root, dirs, files in os.walk(parent_folder_path):
                if "SHP_도로_추출" in root or "SHP_건물_추출" in root:
                    continue
                    
                for file in files:
                    file_name, file_ext = os.path.splitext(file)
                    file_ext_lower = file_ext.lower()
                    
                    if file_name in TARGET_FILES and file_ext_lower in SHP_EXTENSIONS:
                        if not os.path.exists(output_dir):
                            os.makedirs(output_dir)
                            print(f"\n📂 건물 폴더 생성: {output_dir}")
                        
                        # 🔥 중간에 Building_ 네이밍 규칙 추가
                        new_file_name = f"{en_region}_Building_{file_name}{file_ext}"
                        
                        src_file_path = os.path.join(root, file)
                        dst_file_path = os.path.join(output_dir, new_file_name)
                        
                        shutil.copy2(src_file_path, dst_file_path)
                        print(f"   ↳ [건물 복사] {file} -> {new_file_name}")

if __name__ == "__main__":
    print("🚀 [건물 데이터] 추출 및 이름 변경을 시작합니다.")
    extract_building_shp()
    print("\n✅ 건물 데이터 작업이 완료되었습니다.")