import os
import glob
import geopandas as gpd
import pandas as pd
from shapely.validation import make_valid

# runtime warning이 콘솔창을 과도하게 가리면 아래 주석을 해제하세요.
# os.environ["OGR_GEOMETRY_ACCEPT_UNCLOSED_RING"] = "YES"

# ==========================================
# [설정] 도로와 건물 폴더/출력 경로 지정
# ==========================================
ROAD_DIR = r"C:\Users\82103\Documents\RoadSHP"
BUILDING_DIR = r"C:\Users\82103\Documents\BuildingSHP"  # 건물 shp 폴더 경로

ROAD_OUTPUT = r"C:\Users\82103\Documents\RoadSHP\Merged_Roads_Finalv2.gpkg"
BUILDING_OUTPUT = r"C:\Users\82103\Documents\BuildingSHP\Merged_Buildings_Finalv2.gpkg"

TARGET_CRS = "EPSG:5186"
ORIGINAL_CRS = "EPSG:5186"
# ==========================================

def merge_shp_files(input_dir, output_file, job_name):
    print(f"\n==========================================")
    print(f"🎬 [{job_name} 병합 작업 시작]")
    print(f"==========================================")
    
    if os.path.exists(output_file):
        try: os.remove(output_file)
        except Exception: pass

    all_shp_files = glob.glob(os.path.join(input_dir, "*.shp"))
    total_files = len(all_shp_files)
    print(f"🎯 총 {total_files}개의 SHP 파일을 찾았습니다.")

    if total_files == 0:
        print(f"❌ SHP 파일이 없습니다. 경로를 확인하세요: {input_dir}")
        return

    print("🚀 초고속 로드 및 좌표계 변환 중...")
    gdf_list = []
    success_count = 0
    fail_count = 0
    counter = 0

    for file in all_shp_files:
        counter += 1
        try:
            gdf = gpd.read_file(file, engine="pyogrio")
            if not gdf.empty:
                # [추가] 깨진 지오메트리(LinearRing 등) 자동 복구 로직
                if not gdf.geometry.is_valid.all():
                    gdf['geometry'] = gdf.geometry.apply(lambda geom: make_valid(geom) if not geom.is_valid else geom)

                if gdf.crs is None:
                    gdf = gdf.set_crs(ORIGINAL_CRS)
                gdf = gdf.to_crs(TARGET_CRS)
                gdf_list.append(gdf)
                success_count += 1
        except Exception as e:
            print(f"❌ 에러 ({os.path.basename(file)}): {e}")
            fail_count += 1
            continue

        # [추가] 1000개당 1번씩 실시간 진행 상황 메세지 출력
        if counter % 1000 == 0:
            print(f"📊 [진행 상황] {counter}/{total_files} 완료 | 성공: {success_count}개, 건너뜀: {fail_count}개")

    if not gdf_list:
        print("❌ 변환된 데이터가 없습니다.")
        return

    print(f"\n🏁 1차 로드 완료! (최종 성공: {success_count}개, 최종 실패: {fail_count}개)")
    print("데이터셋 하나로 통합 중...")
    merged_gdf = gpd.GeoDataFrame(pd.concat(gdf_list, ignore_index=True), crs=TARGET_CRS)

    print("최종 GeoPackage 파일 물리 저장 중...")
    merged_gdf.to_file(output_file, layer=job_name.lower(), driver="GPKG", engine="pyogrio")

    if os.path.exists(output_file):
        final_size_mb = os.path.getsize(output_file) / (1024 * 1024)
        print(f"🎉 [{job_name} 완료] 용량: {final_size_mb:.2f} MB")
    else:
        print(f"❌ [{job_name}] 저장에 실패했습니다.")

# 1. 도로 병합 실행
merge_shp_files(ROAD_DIR, ROAD_OUTPUT, "Roads")

# 2. 건물 병합 실행 (도로가 끝나면 자동으로 바로 시작)
merge_shp_files(BUILDING_DIR, BUILDING_OUTPUT, "Buildings")

print("\n✨ 모든 병합 작업이 완전히 끝났습니다!")