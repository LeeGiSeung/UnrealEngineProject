const express = require('express');
const app = express();
const port = 3000;

app.use(express.json());

const mongoose = require('mongoose');

mongoose.connect('mongodb://127.0.0.1:27017/projectV')
    .then(() => console.log("MongoDB 연결 성공"))
    .catch(err => console.log(err));

// 초기 데이터 (FSkillInfo 구조에 맞춘 샘플)
let playerSkillData = {
    // 언리얼에서 요청하는 ID (0 또는 player_01)
    "player_01": {
        "CharacterId": "CH_V_001",
        "CharacterName": "Aria",
        
        // 1. 메인 스탯 정보
        "Maininfo": {
            "Level": 60,
            "LevelEXP": 0.3,
            "HP": 1250.5,
            "Attack": 210.0,
            "Defence": 85.5,
            "Force": 150.0,
            "Critical": 15.5,
            "CriticalDamage": 50.0
        },

        // 2. 유물 정보 (각 Part별 상세 데이터)
        "Relicinfo": {
            "Part_0": {
                "RelicId": "Relic.Four.Redskull",
                "RelicImageId": "Relic.Four.Redskull",
                "Level": 3,
                "HP": 150.0,
                "Attack": 10.0,
                "Defence": 25.0,
                "Force": 5.0,
                "Critical": 2.5,
                "CriticalDamage": 10.0
            },
            "Part_1": {
                "RelicId": "Relic.Three.RedSkull",
                "RelicImageId": "Relic.Three.RedSkull",
                "Level": 2,
                "HP": 120.0,
                "Attack": 5.0,
                "Defence": 20.0,
                "Force": 3.0,
                "Critical": 1.0,
                "CriticalDamage": 5.0
            },
            "Part_2": {
                "RelicId": "Relic.Three.BlueSkull",
                "RelicImageId": "Relic.Three.BlueSkull",
                "Level": 4,
                "HP": 80.0,
                "Attack": 15.0,
                "Defence": 10.0,
                "Force": 6.0,
                "Critical": 3.0,
                "CriticalDamage": 8.0
            },
            "Part_3": {
                "RelicId": "Relic.One.RedSkull",
                "RelicImageId": "Relic.One.RedSkull",
                "Level": 1,
                "HP": 60.0,
                "Attack": 2.0,
                "Defence": 12.0,
                "Force": 2.0,
                "Critical": 0.5,
                "CriticalDamage": 2.0
            },
            "Part_4": {
                "RelicId": "Relic.One.BlueSkull",
                "RelicImageId": "Relic.One.BlueSkull",
                "Level": 5,
                "HP": 200.0,
                "Attack": 20.0,
                "Defence": 15.0,
                "Force": 10.0,
                "Critical": 5.0,
                "CriticalDamage": 20.0
            }
        },

        // 3. 스킬 정보 (레벨 및 노드 활성화)
        "SkillInfo": {
            "AutoAttackLevel": 5,
            "AutoAttackNode_0": true,
            "AutoAttackNode_1": true,
            "AutoAttackNode_2": false,
            "EAttackLevel": 3,
            "EAttackkNode_0": true, // 구조체 오타 반영
            "EAttackNode_1": false,
            "EAttackNode_2": false,
            "RAttackLevel": 1,
            "RAttackNode_0": false,
            "RAttackNode_1": false,
            "RAttackNode_2": false,
            "PAttackLevel": 2,
            "PAttackNode_0": true,
            "PAttackNode_1": true,
            "PAttackNode_2": false
        },

        // 4. 성급/돌파 정보
        "StarInfo": {
            "CharacterID": "BaseCharacter",
            "StarLevel": 3
        }
    }
};

// 예비 유물 데이터 따로 저장
let playerInventoryRelics = {
    "player_01": [
        {
            "RelicUID": "relic_001",
            "RelicId": "Relic.Three.BlueSkull",
            "RelicImageId": "Relic.Three.BlueSkull",
            "Level": 4,
            "HP": 80.0,
            "Attack": 15.0,
            "Defence": 10.0,
            "Force": 6.0,
            "Critical": 3.0,
            "CriticalDamage": 8.0
        },
        {
            "RelicUID": "relic_002",
            "RelicId": "Relic.Four.RedSkull",
            "RelicImageId": "Relic.Four.RedSkull",
            "Level": 5,
            "HP": 200.0,
            "Attack": 25.0,
            "Defence": 20.0,
            "Force": 10.0,
            "Critical": 5.0,
            "CriticalDamage": 15.0
        }
    ]
};

// [GET] 캐릭터의 모든 스킬 정보 조회
app.get('/api/skills/:playerId', (req, res) => {
    const playerId = req.params.playerId;
    const data = playerSkillData[playerId];

    if (data) {
        console.log(`[조회] ${playerId}의 스킬 데이터를 전송합니다.`);
        res.json(data);
    } else {
        res.status(404).json({ message: "Character not found" });
    }
});

// [POST] 캐릭터의 스킬 정보 전체 저장 (업데이트)
app.post('/api/skills/:playerId', (req, res) => {
    const playerId = req.params.playerId;
    const newData = req.body; // 언리얼에서 보낸 FSkillInfo JSON

    // 데이터 저장 (메모리)
    playerSkillData[playerId] = newData;

    console.log(`[저장] ${playerId}의 스킬 정보가 업데이트되었습니다.`);
    console.log(newData); // 터미널에서 바뀐 내용 확인

    res.status(200).json({ message: "Success", savedData: newData });
});

app.get('/api/relics/:playerId', (req, res) => {
    const playerId = req.params.playerId;
    const data = playerInventoryRelics[playerId];

    if (data) {
        console.log(`[조회] ${playerId}의 예비 유물 데이터를 전송합니다.`);
        res.json({ InventoryRelics: data });
    } else {
        res.status(404).json({ message: "Inventory not found" });
    }
});

app.post('/api/relics/:playerId', (req, res) => {
    const playerId = req.params.playerId;
    const data = playerInventoryRelics[playerId];

    if (data) {
        console.log(`[조회-POST] ${playerId}의 예비 유물 데이터를 업데이트합니다.`);
        res.json({ InventoryRelics: data });

        playerInventoryRelics[playerId] = req.body.inventoryRelics;

    } else {
        res.status(404).json({ message: "Inventory not found" });
    }
});

app.listen(port, () => {
    console.log(`🚀 ProjectV 스킬 서버가 http://localhost:${port} 에서 실행 중입니다!`);
});