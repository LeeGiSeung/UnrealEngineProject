from PIL import Image
import numpy as np
from torchvision import datasets, transforms, models
import onnxruntime as ort
import torch

train_transform = transforms.Compose([
    transforms.Grayscale(num_output_channels=3),
    transforms.RandomRotation(20),
    transforms.RandomAffine(0, translate=(0.1, 0.1)),
    transforms.RandomResizedCrop(224),
    transforms.RandomHorizontalFlip(),
    transforms.ToTensor(),
])

val_transform = transforms.Compose([
    transforms.Grayscale(num_output_channels=3),
    transforms.Resize((224, 224)),
    transforms.ToTensor(),
])

# ONNX 모델 로드
session = ort.InferenceSession("shape_feature_ir8.onnx")

input_name = session.get_inputs()[0].name

def extract_feature_onnx(img_path):
    image = Image.open(img_path)
    tensor = val_transform(image).unsqueeze(0)

    input_data = tensor.numpy().astype(np.float32)

    outputs = session.run(None, {input_name: input_data})

    feature = outputs[0].squeeze(0)  # [1280]

    print(f"ONNX Output shape ({img_path}):", outputs[0].shape)
    print(f"Feature sample ({img_path}):", feature[:10])

    return feature

# 두 이미지
img1 = "Capture.png"
img2 = "CaptureImg.png"

feat1 = extract_feature_onnx(img1)
feat2 = extract_feature_onnx(img2)

# 코사인 유사도 계산 (numpy)
cos_sim = np.dot(feat1, feat2) / (
    np.linalg.norm(feat1) * np.linalg.norm(feat2)
)

print("\nCosine Similarity (ONNX):", cos_sim)