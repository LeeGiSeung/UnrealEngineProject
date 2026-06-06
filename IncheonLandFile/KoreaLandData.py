import os
import zipfile

def extract_zips_excluding_some_folders(target_directory):
    # 제외하고 싶은 폴더 이름을 세트로 지정 (속도 향상 및 비교를 위해)
    exclude_folders = {'1', '2', '3'}
    
    zip_count = 0
    
    # topdown=True로 설정해야 하위 폴더 탐색을 제어할 수 있습니다.
    for root, dirs, files in os.walk(target_directory, topdown=True):
        
        # [핵심] 현재 탐색 중인 폴더 위치 바로 아래에 있는 폴더들 중
        # '1', '2', '3'에 해당하는 폴더가 있다면 탐색 목록(dirs)에서 아예 제외해 버립니다.
        dirs[:] = [d for d in dirs if d not in exclude_folders]
        
        for file in files:
            # 파일 확장자가 .zip인 경우만 처리
            if file.lower().endswith('.zip'):
                zip_count += 1
                zip_file_path = os.path.join(root, file)
                
                # 압축 파일이 있는 그 위치에 파일 이름으로 폴더 생성
                extract_to_dir = os.path.splitext(zip_file_path)[0]
                
                print(f"[{zip_count}] 압축 해제 중: {zip_file_path}")
                
                try:
                    with zipfile.ZipFile(zip_file_path, 'r') as zip_ref:
                        # 한글 깨짐 방지 디코딩
                        for member in zip_ref.infolist():
                            try:
                                filename = member.filename.encode('cp437').decode('cp949')
                            except (UnicodeEncodeError, UnicodeDecodeError):
                                filename = member.filename
                            
                            target_path = os.path.join(extract_to_dir, filename)
                            
                            if member.is_dir():
                                os.makedirs(target_path, exist_ok=True)
                            else:
                                os.makedirs(os.path.dirname(target_path), exist_ok=True)
                                with zip_ref.open(member) as source, open(target_path, "wb") as target:
                                    target.write(source.read())
                                    
                    print(f"   => ✅ 완료")
                except Exception as e:
                    print(f"   => ❌ 에러 발생: {e}")

    if zip_count == 0:
        print("조건에 맞는 .zip 파일이 발견되지 않았습니다.")
    else:
        print(f"\n🎉 총 {zip_count}개의 압축 파일 처리가 완료되었습니다!")

# 실행 코드
target_folder = r"D:\KoreaLandInfo"
extract_zips_excluding_some_folders(target_folder)