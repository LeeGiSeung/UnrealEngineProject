import os
import shutil
from korean_romanizer.romanizer import Romanizer

ROOT_DIR = r"D:\KoreaLandInfo\Nanum"

BUILDING_OUTPUT_DIR = os.path.join(ROOT_DIR, "BuildingSHP")
ROAD_OUTPUT_DIR = os.path.join(ROOT_DIR, "RoadSHP")

SHP_EXTENSIONS = [".shp", ".shx", ".dbf", ".prj", ".qpj", ".cpg"]

BUILDING_FILES = {
    "N3A_B0010000"
}

ROAD_FILES = {
    "N3L_A0020000",
    "N3L_A0010000",
    "N3A_A0033320"
}


def clean_and_romanize(text):
    romanized = Romanizer(text).romanize()
    return romanized.replace(" ", "_").lower()


def ensure_dir(path):
    if not os.path.exists(path):
        os.makedirs(path)


def copy_shp_file(src_file_path,
                  dst_dir,
                  region_name,
                  tile_name,
                  category,
                  original_name,
                  ext):

    new_file_name = (
        f"{region_name}_{tile_name}_{category}_{original_name}{ext}"
    )

    dst_file_path = os.path.join(dst_dir, new_file_name)

    try:
        shutil.copy2(src_file_path, dst_file_path)
    except Exception as e:
        print(f"❌ 복사 실패: {src_file_path}")
        print(e)


def extract_all_shp():

    ensure_dir(BUILDING_OUTPUT_DIR)
    ensure_dir(ROAD_OUTPUT_DIR)

    sub_folders = [
        f for f in os.listdir(ROOT_DIR)
        if os.path.isdir(os.path.join(ROOT_DIR, f))
        and f.startswith("2MAP5000_SHP_")
    ]

    total_folders = len(sub_folders)

    print("🚀 건물 + 도로 데이터 통합 추출 시작")
    print("--------------------------------------------------")

    for idx, folder_name in enumerate(sub_folders, 1):

        ko_region = folder_name.replace("2MAP5000_SHP_", "")
        en_region = clean_and_romanize(ko_region)

        parent_folder_path = os.path.join(ROOT_DIR, folder_name)

        print(
            f"[{idx}/{total_folders}] "
            f"{ko_region} ({en_region})"
        )

        for root, dirs, files in os.walk(parent_folder_path):

            if (
                "SHP_도로_추출" in root
                or "SHP_건물_추출" in root
                or "RoadSHP" in root
                or "BuildingSHP" in root
            ):
                continue

            tile_name = os.path.basename(root)

            for file in files:

                file_name, ext = os.path.splitext(file)

                if ext.lower() not in SHP_EXTENSIONS:
                    continue

                src_file_path = os.path.join(root, file)

                # 건물
                if file_name in BUILDING_FILES:
                    copy_shp_file(
                        src_file_path,
                        BUILDING_OUTPUT_DIR,
                        en_region,
                        tile_name,
                        "Building",
                        file_name,
                        ext
                    )

                # 도로
                elif file_name in ROAD_FILES:
                    copy_shp_file(
                        src_file_path,
                        ROAD_OUTPUT_DIR,
                        en_region,
                        tile_name,
                        "Road",
                        file_name,
                        ext
                    )


if __name__ == "__main__":
    import time

    start_time = time.time()

    extract_all_shp()

    print("--------------------------------------------------")
    print(f"✅ 완료")
    print(f"⏱️ {time.time() - start_time:.2f}초")