const express = require('express');
const mongoose = require('mongoose');
const app = express();
const port = 3000;

app.use(express.json());

// MongoDB 연결
mongoose.connect('mongodb://127.0.0.1:27017/projectV')
    .then(() => console.log("✅ MongoDB 연결 성공"))
    .catch(err => console.error("❌ 연결 실패:", err));

// [Schema] 요청하신 모든 필드 (대소문자 중복 포함) 정의
const PlayerSchema = new mongoose.Schema({
playerId: { type: String, required: true, unique: true },
    characterId: String,
    characterName: String,
    maininfo: Object,
    relicinfo: Object,
    skillInfo: Object,
    starInfo: Object,
    InventoryRelics: Array  // 오직 대문자 필드만 사용
}, { strict: false });

const Player = mongoose.model('Player', PlayerSchema);

                // maininfo: {
                //     Level: 60,
                //     LevelEXP: 0.3,
                //     HP: 1250.5,
                //     Attack: 210,
                //     Defence: 85.5,
                //     Force: 150,
                //     Critical: 15.5,
                //     CriticalDamage: 50
                // },

/**
 * [초기화 함수] DB에 데이터가 없으면 요청하신 JSON 구조로 생성
 */
async function initializeDatabase() {
    try {
        const playerId = "player_01";
        const existingPlayer = await Player.findOne({ playerId });

        if (!existingPlayer) {
            console.log("⚠️ 데이터가 없습니다. 요청하신 구조로 초기 데이터를 생성합니다...");

            const initialData = {
                playerId: playerId,
                characterId: "CH_V_001",
                characterName: "Aria",
                maininfo: {
                    Level: 51,
                    LevelEXP: 0,
                    HP: 100,
                    Attack: 10,
                    Defence: 10,
                    Force: 5,
                    Critical: 5,
                    CriticalDamage: 50
                },
                relicinfo: {
                    Part_0: { RelicUID : "relic_001", RelicId: "Relic.Four.RedSkull", RelicImageId: "Relic.Four.RedSkull", Level: 3, HP: 150, Attack: 10, Defence: 25, Force: 5, Critical: 2.5, CriticalDamage: 10 },
                    Part_1: { RelicUID : "relic_004", RelicId: "Relic.Three.RedSkull", RelicImageId: "Relic.Three.RedSkull", Level: 2, HP: 120, Attack: 5, Defence: 20, Force: 3, Critical: 1, CriticalDamage: 5 },
                    Part_2: { RelicUID : "relic_003", RelicId: "Relic.Three.BlueSkull", RelicImageId: "Relic.Three.BlueSkull", Level: 4, HP: 80, Attack: 15, Defence: 10, Force: 6, Critical: 3, CriticalDamage: 8 },
                    Part_3: { RelicUID : "relic_005", RelicId: "Relic.One.RedSkull", RelicImageId: "Relic.One.RedSkull", Level: 1, HP: 60, Attack: 2, Defence: 12, Force: 2, Critical: 0.5, CriticalDamage: 2 },
                    Part_4: { RelicUID : "relic_006", RelicId: "Relic.One.BlueSkull", RelicImageId: "Relic.One.BlueSkull", Level: 5, HP: 200, Attack: 20, Defence: 15, Force: 10, Critical: 5, CriticalDamage: 20 }
                },
                skillInfo: {
                    AutoAttackLevel: 5, AutoAttackNode_0: true, AutoAttackNode_1: true, AutoAttackNode_2: false,
                    EAttackLevel: 3, EAttackkNode_0: true, EAttackNode_1: false, EAttackNode_2: false,
                    RAttackLevel: 1, RAttackNode_0: false, RAttackNode_1: false, RAttackNode_2: false,
                    PAttackLevel: 2, PAttackNode_0: true, PAttackNode_1: true, PAttackNode_2: false
                },
                starInfo: {
                    CharacterID: "BaseCharacter",
                    StarLevel: 3
                },

                InventoryRelics: [
                    { RelicUID: "relic_003", RelicId: "Relic.Three.BlueSkull", RelicImageId: "Relic.Three.BlueSkull", Level: 4, HP: 80, Attack: 15, Defence: 10, Force: 6, Critical: 3, CriticalDamage: 8 },
                    { RelicUID: "relic_001", RelicId: "Relic.Four.RedSkull", RelicImageId: "Relic.Four.RedSkull", Level: 5, HP: 200, Attack: 25, Defence: 20, Force: 10, Critical: 5, CriticalDamage: 15 },
                    { RelicUID: "relic_001", RelicId: "Relic.Four.RedSkull", RelicImageId: "Relic.Four.RedSkull", Level: 5, HP: 200, Attack: 25, Defence: 20, Force: 10, Critical: 5, CriticalDamage: 15 }

                ],
            };

            await Player.create(initialData);
            console.log("✅ 초기 데이터 생성 완료 (레벨 62 / 이중 인벤토리)");
        }
    } catch (err) {
        console.error("❌ 초기화 오류:", err);
    }
}

// [POST] 인벤토리(유물) 업데이트 전용
app.post('/api/relics/:playerId', async (req, res) => {
    console.log("🎒 인벤토리(relics) 데이터 post 성공");

    try {
        const playerId = req.params.playerId;
        
        // 언리얼에서 보낸 데이터 본문을 가져옵니다.
        // 언리얼에서 'InventoryRelics'라는 이름으로 배열을 보낸다고 가정합니다.
        const { inventoryRelics } = req.body;

        console.log(req.body);

        const updated = await Player.findOneAndUpdate(
            { playerId: playerId },
            { $set: { InventoryRelics: inventoryRelics } }, // 인벤토리 필드만 정밀 타격해서 수정
            { new: true, upsert: true }
        );

        res.json({ message: "Inventory Update Success", savedData: updated });
    } catch (err) {
        console.error("❌ 인벤토리 업데이트 에러:", err);
        res.status(500).json({ error: err.message });
    }
});

// [GET] 인벤토리(유물) 데이터만 조회
app.get('/api/relics/:playerId', async (req, res) => {
    console.log("🎒 인벤토리(relics) 데이터 get 요청 감지");

    try {
        const playerId = req.params.playerId;
        // DB에서 해당 유저를 찾는데, InventoryRelics 필드만 골라서 가져옵니다.
        const player = await Player.findOne({ playerId: playerId }, 'InventoryRelics');



        if (player) {
            // 인벤토리 배열만 깔끔하게 응답
            console.log("인벤토리 Get 요청 성공");
            console.log("응답 보냄:", player.InventoryRelics);
            res.json({ InventoryRelics: player.InventoryRelics });
        } else {
            res.status(404).json({ message: "Player Not Found" });
        }
    } catch (err) {
        console.error("❌ 인벤토리 조회 에러:", err);
        res.status(500).json({ error: err.message });
    }
});

// DB 연결 시 초기화 함수 실행
mongoose.connection.once('open', initializeDatabase);

// [GET] 전체 정보 조회
app.get('/api/skills/:playerId', async (req, res) => {

    console.log(" chracter data get 성공");

    try {
        
        const player = await Player.findOne({ playerId: req.params.playerId });
        player ? res.json(player) : res.status(404).json({ message: "Not Found" });
    } catch (err) {
        res.status(500).json({ error: err.message });
    }
});

// [POST] 캐릭터 스탯 업데이트
app.post('/api/skills/:playerId', async (req, res) => {

console.log(" chracter data post 성공");

    try {
        
        const updated = await Player.findOneAndUpdate(
            { playerId: req.params.playerId },
            { $set: req.body },
            { new: true, upsert: true }
        );
        res.json({ message: "Success", savedData: updated });
    } catch (err) {
        res.status(500).json({ error: err.message });
    }
});

const http = require('http');
const WebSocket = require('ws'); // 표준 웹소켓 라이브러리 추가

const server = http.createServer(app);

// 1. 표준 WebSocket 서버 설정 (Socket.io 대신 또는 함께 사용)
const wss = new WebSocket.Server({ server }); 

// 연결된 모든 언리얼 클라이언트를 관리하기 위한 세트
const ueClients = new Set();

wss.on('connection', (ws) => {
    console.log('ProjectV 언리얼 클라이언트 접속 성공!');
    ueClients.add(ws);

    ws.on('close', () => {
        ueClients.delete(ws);
        console.log('클라이언트 접속 종료');
    });
});

app.use(express.json());

// WPF가 호출하는 경로
app.post('/update-player', async (req, res) => {
    try {
        // 1. MongoDB 데이터 수정 로직 (생략)
        console.log("데이터 변경 감지! 모든 UE5 클라이언트에게 즉시 알림을 보냅니다.");

        // 2. 표준 웹소켓으로 연결된 모든 언리얼에 신호 전송
        const message = "REFRESH_STATS";
        ueClients.forEach(client => {
            if (client.readyState === WebSocket.OPEN) {
                client.send(message);
            }
        });

        res.status(200).send("Success");
    } catch (error) {
        res.status(500).send("Error");
    }
});

app.listen(port, () => {
    console.log(`🚀 서버 실행 중: http://localhost:${port}`);
});