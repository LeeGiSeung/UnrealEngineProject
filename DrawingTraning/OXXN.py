import torch
import torch.nn as nn
import torch.optim as optim
from torchvision import datasets, transforms, models
from torch.utils.data import DataLoader

# ===== 설정 =====
DATA_DIR = "dataset"
BATCH_SIZE = 16
EPOCHS = 15
DEVICE = "cuda" if torch.cuda.is_available() else "cpu"

# ===== 데이터 증강 =====
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

train_dataset = datasets.ImageFolder(f"{DATA_DIR}/Train", transform=train_transform)
val_dataset = datasets.ImageFolder(f"{DATA_DIR}/Val", transform=val_transform)

train_loader = DataLoader(train_dataset, batch_size=BATCH_SIZE, shuffle=True)
val_loader = DataLoader(val_dataset, batch_size=BATCH_SIZE)

class_names = train_dataset.classes
print("Classes:", class_names)

# ===== 모델 =====
model = models.mobilenet_v2(pretrained=True)

# 마지막 레이어 교체 (3클래스)
model.classifier[1] = nn.Linear(model.last_channel, 4)

# feature freeze (데이터 적을 때 필수)
for param in model.features.parameters():
    param.requires_grad = False

model = model.to(DEVICE)

criterion = nn.CrossEntropyLoss()
optimizer = optim.Adam(model.classifier.parameters(), lr=0.001)

# ===== 학습 =====
for epoch in range(EPOCHS):
    model.train()
    total_loss = 0

    for images, labels in train_loader:
        images, labels = images.to(DEVICE), labels.to(DEVICE)

        optimizer.zero_grad()
        outputs = model(images)
        loss = criterion(outputs, labels)
        loss.backward()
        optimizer.step()

        total_loss += loss.item()

    print(f"Epoch {epoch+1}/{EPOCHS}, Loss: {total_loss:.4f}")

# ===== Feature extractor 모델 =====
class FeatureExtractor(nn.Module):
    def __init__(self, base_model):
        super().__init__()
        self.features = base_model.features
        self.pool = nn.AdaptiveAvgPool2d((1, 1))

    def forward(self, x):
        x = self.features(x)
        x = self.pool(x)
        x = torch.flatten(x, 1)   # 🔥 이게 중요
        return x

feature_model = FeatureExtractor(model).to(DEVICE)
feature_model.eval()

dummy_input = torch.randn(1, 3, 224, 224).to(DEVICE)

torch.onnx.export(
    feature_model,
    dummy_input,
    "Class4.onnx",
    input_names=["input"],
    output_names=["features"],
    opset_version=11
)

print("Feature ONNX exported!")

import onnx

model = onnx.load("Class4.onnx")

for output in model.graph.output:
    print(output.type.tensor_type.shape)


from PIL import Image
import torch

# 모델 eval 모드
feature_model.eval()

# 이미지 로드
img_path = "Capture.png"
image = Image.open(img_path)

# val_transform 사용
tensor = val_transform(image).unsqueeze(0)  # 배치 차원 추가

with torch.no_grad():
    output = feature_model(tensor)

print("Output shape Near :", output.shape)
print("Feature vector sample Near:", output[0][:10])