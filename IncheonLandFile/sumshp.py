
import sys
import subprocess

# 코드 실행창 안에서 직접 geopandas와 pandas 설치 명령 내리기
subprocess.check_call([sys.executable, "-m", "pip", "install", "geopandas", "shapely", "fiona", "pandas"])
import os
import glob
import geopandas as gpd
import pandas as pd

# 1. 수만 개의 SHP 파일이 들어있는 폴더 경로 지정 (본인의 경로로 수정하세요)
# 예: r"C:\Users\username\Desktop\busan_data"
input_folder = r"D:\KoreaLandInfo\Nanum\BuildingSHP"

# 2. 결과물을 저장할 파일 경로 및 이름 지정
output_file = os.path.join(input_folder, "merged_Building.shp")

# 폴더 내 모든 .shp 파일 목록 가져오기
shp_files = glob.glob(os.path.join(input_folder, "*.shp"))
total_files = len(shp_files)
print(f"총 {total_files}개의 SHP 파일을 찾았습니다. 병합을 시작합니다...")

# 3. 효율적인 병합을 위해 리스트에 각 파일의 데이터프레임을 담기
gdf_list = []

for idx, file in enumerate(shp_files, 1):
    try:
        # 파일 읽기 (동일 폴더의 .dbf, .prj 등을 파이썬이 알아서 같이 읽습니다)
        gdf = gpd.read_file(file)
        
        # 데이터가 비어있지 않은 경우에만 추가
        if not gdf.empty:
            gdf_list.append(gdf)
            
        if idx % 5000 == 0:
            print(f"진행 상황: {idx} / {total_files} 완료...")
    except Exception as e:
        print(f"파일 읽기 오류 ({os.path.basename(file)}): {e}")

# 4. 하나로 합치기 (Concat)
if gdf_list:
    print("모든 파일을 하나로 결합하는 중...")
    merged_gdf = gpd.GeoDataFrame(pd.concat(gdf_list, ignore_index=True), crs=gdf_list[0].crs)
    
    print("최종 SHP 파일로 저장 중... (이 과정은 수 분이 소요될 수 있습니다)")
    # 한글 깨짐 방지를 위해 인코딩을 cp949 또는 utf-8로 지정합니다.
    merged_gdf.to_file(output_file, encoding='cp949')
    print(f"★ 병합 완료! 결과 파일: {output_file}")
else:
    print("합칠 데이터가 없습니다.")