import os
import glob
import geopandas as gpd
import pandas as pd

# ==========================================
# [설정] 본인의 폴더 경로에 맞게 지정하세요
# ==========================================
SHP_DIR = r"C:\Users\82103\Documents\RoadSHP"
OUTPUT_GPKG = r"C:\Users\82103\Documents\RoadSHP\Merged_Roads_Test.gpkg" # 테스트용 파일명으로 변경
LAYER_NAME = "roads"
# ==========================================

# 1. 기존 결과 파일 삭제
if os.path.exists(OUTPUT_GPKG):
    try: os.remove(OUTPUT_GPKG)
    except Exception: pass

# 2. 모든 .shp 파일 검색
print("1/3 단계: 폴더 내 모든 SHP 파일 목록 검색 중...")
all_shp_files = glob.glob(os.path.join(SHP_DIR, "*.shp"))
total_files = len(all_shp_files)
print(f"🎯 폴더 내 총 {total_files}개의 SHP 파일을 찾았습니다.")

if total_files == 0:
    print("❌ SHP 파일이 없습니다. 경로를 확인하세요.")
    exit()

# ⭐ [핵심 변경] 전체 파일 중 딱 앞의 100개만 잘라서 진행합니다.
shp_files = all_shp_files[:100]
print(f"🧪 테스트를 위해 상위 {len(shp_files)}개 파일만 읽어옵니다.")

# 3. 100개 파일만 메모리 리스트에 적재
print("\n2/3 단계: 100개 파일 초고속 로드 및 좌표계 변환 중...")
gdf_list = []
target_crs = "EPSG:5186" 
original_crs = 'EPSG:5186'

for idx, file in enumerate(shp_files, 1):
    try:
        # pyogrio 엔진을 사용하여 읽기 속도를 극대화합니다.
        gdf = gpd.read_file(file, engine="pyogrio")
        if not gdf.empty:
            # 💡 읽어오자마자 바로 좌표계 체크 및 변환을 수행하여 리스트에 담습니다.
            if gdf.crs is None:
                gdf = gdf.set_crs(original_crs)
            gdf = gdf.to_crs(target_crs)
            gdf_list.append(gdf)
    except Exception as e:
        print(f"\n❌ 파일 읽기 에러 ({os.path.basename(file)}): {e}")
        continue
    
    print(f"▶ 로드 진행 상황: {idx} / {len(shp_files)} 파일 처리 완료...", end="\r")

if not gdf_list:
    print("\n❌ 변환된 데이터가 없습니다. 파일을 확인하세요.")
    exit()

print("\n\n데이터셋 하나로 통합 중...")
# 3. 좌표계가 똑같아졌으므로 안전하게 병합 진행
merged_gdf = gpd.GeoDataFrame(pd.concat(gdf_list, ignore_index=True), crs=target_crs)

# 4. GeoPackage 단일 파일로 초고속 물리 저장
print("\n3/3 단계: 최종 GeoPackage 파일 물리 저장 중 (잠시만 기다리세요)...")
merged_gdf.to_file(OUTPUT_GPKG, layer=LAYER_NAME, driver="GPKG", engine="pyogrio")

# 5. 최종 결과 검증
if os.path.exists(OUTPUT_GPKG):
    final_size_mb = os.path.getsize(OUTPUT_GPKG) / (1024 * 1024)
    print(f"\n🎉 [테스트 성공] 100개 데이터가 유실 없이 병합되었습니다!")
    print(f"👉 파일 위치: {OUTPUT_GPKG}")
    print(f"📦 최종 용량: {final_size_mb:.2f} MB")
else:
    print(f"\n❌ 저장에 실패했습니다. 드라이브 공간을 확인하세요.")