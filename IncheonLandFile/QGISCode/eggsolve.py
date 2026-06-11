import os
import zipfile

ROOT_DIR = r"D:\KoreaLandInfo\1"

zip_count = 0

for region_folder in os.listdir(ROOT_DIR):

    region_path = os.path.join(ROOT_DIR, region_folder)

    if not os.path.isdir(region_path):
        continue

    print(f"처리중: {region_folder}")

    for file_name in os.listdir(region_path):

        if not file_name.lower().endswith(".zip"):
            continue

        zip_count += 1

        zip_path = os.path.join(region_path, file_name)

        extract_dir = os.path.join(
            region_path,
            os.path.splitext(file_name)[0]
        )

        try:
            os.makedirs(extract_dir, exist_ok=True)

            with zipfile.ZipFile(zip_path, "r") as zip_ref:
                zip_ref.extractall(extract_dir)

            #print(f"[{zip_count}] 완료 : {file_name}")

        except Exception as e:
            print(f"[{zip_count}] 실패 : {file_name}")
            print(e)

print(f"\n총 {zip_count}개 ZIP 처리 완료")