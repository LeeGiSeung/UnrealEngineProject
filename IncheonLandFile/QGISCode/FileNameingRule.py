import os
import shutil

# ================= [ 사용자 설정 영역 ] =================
# 실제 IncheonMichuolPNG 폴더가 있는 절대 경로를 입력하세요.
BASE_DIR = r"C:\Users\82103\Documents\Unreal Projects\ProjectV\IncheonLandFile\IncheonMichuolPNG"

# 13레벨의 "실제 기준이 되어야 하는 시작 폴더(X)와 시작 파일(Y)"을 정의합니다.
START_LEVEL = 13
BASE_X_13 = 6977
BASE_Y_13 = 3174
# ========================================================

def fix_tile_coordinates_fixed_y():
    # 임시 변환을 위한 작업 폴더 생성
    temp_dir = os.path.join(os.path.dirname(BASE_DIR), "IncheonMichuolPNG_Fixed")
    if os.path.exists(temp_dir):
        shutil.rmtree(temp_dir)
    os.makedirs(temp_dir, exist_ok=True)

    print("🚀 [업데이트 버전] 타일 이름 및 구조 교정 시작...")

    for level in range(13, 18):
        level_str = str(level)
        src_level_path = os.path.join(BASE_DIR, level_str)
        
        if not os.path.exists(src_level_path):
            print(f"⚠️ {level} 레벨 폴더가 존재하지 않아 건너뜁니다.")
            continue

        # 줌 레벨 배수 법칙에 따른 현재 레벨의 절대적인 시작 X, Y 계산
        multiplier = 2 ** (level - START_LEVEL)
        correct_start_x = BASE_X_13 * multiplier
        correct_start_y = BASE_Y_13 * multiplier

        print(f"\n📁 [레벨 {level}] 고정 기준선 -> 시작 X: {correct_start_x}, 시작 Y: {correct_start_y}")

        # 현재 레벨 안의 X 폴더들을 숫자로 정렬하여 가져옴
        try:
            wrong_x_folders = sorted([int(f) for f in os.listdir(src_level_path) if f.isdigit()])
        except Exception:
            continue

        if not wrong_x_folders:
            continue

        # 가장 번호가 낮은 폴더를 찾아내어 기준 오프셋 0으로 설정
        actual_min_x = wrong_x_folders[0]

        for wrong_x in wrong_x_folders:
            # X 오프셋 계산 후 올바른 X 폴더 번호 확정
            offset_x = wrong_x - actual_min_x
            correct_x = correct_start_x + offset_x
            
            src_x_path = os.path.join(src_level_path, str(wrong_x))
            dst_x_path = os.path.join(temp_dir, level_str, str(correct_x))
            os.makedirs(dst_x_path, exist_ok=True)

            # 해당 X 폴더 안의 png 파일들 수집
            png_files = [f for f in os.listdir(src_x_path) if f.endswith('.png')]
            if not png_files:
                continue

            # 파일명(Y)을 숫자로 정렬 (예: 3174, 3175, 3176 순서 보장)
            try:
                wrong_y_numbers = sorted([int(f.split('.')[0]) for f in png_files if f.split('.')[0].isdigit()])
            except ValueError:
                continue

            if not wrong_y_numbers:
                continue

            # 중요: 모든 X 폴더의 내부 Y 파일들은 구조가 같으므로, 
            # 해당 폴더에서 가장 번호가 낮은 파일(actual_min_y)이 무조건 correct_start_y가 됩니다.
            actual_min_y = wrong_y_numbers[0]

            for wrong_y in wrong_y_numbers:
                # Y 오프셋 계산 후 올바른 Y 파일 번호 확정
                offset_y = wrong_y - actual_min_y
                correct_y = correct_start_y + offset_y

                # 파일 복사 생성 (예: 27909.png 위치에 있던 파일이 -> 27908.png 이름으로 복사됨)
                src_file_path = os.path.join(src_x_path, f"{wrong_y}.png")
                dst_file_path = os.path.join(dst_x_path, f"{correct_y}.png")
                
                shutil.copy2(src_file_path, dst_file_path)

    # 최종 폴더 교체
    print("\n🔄 원본 폴더를 교정된 폴더로 교체하는 중...")
    try:
        shutil.rmtree(BASE_DIR)
        shutil.move(temp_dir, BASE_DIR)
        print("✨ 모든 타일 폴더 및 파일명이 배수 법칙(*2)에 맞게 완벽히 정렬되었습니다!")
    except Exception as e:
        print(f"❌ 폴더 교체 중 오류 발생: {e}")
        print(f"교정된 파일 보관 위치: {temp_dir}")

if __name__ == "__main__":
    fix_tile_coordinates_fixed_y()