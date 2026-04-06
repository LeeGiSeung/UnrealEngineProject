const express = require('express');
const app = express();
const port = 3000;

// JSON 본문을 파싱하기 위한 미들웨어
app.use(express.json());

// 임시 데이터 (나중에 DB로 대체)
let userSkills = {
    "player_01": [
        { "skillId": "Meteor", "level": 1 },
        { "skillId": "FireBall", "level": 3 }
    ]
};

// [GET] 스킬 정보 불러오기
app.get('/api/skills/:playerId', (req, res) => {
    const playerId = req.params.playerId;
    const skills = userSkills[playerId] || [];
    console.log(`${playerId}의 데이터를 조회합니다.`);
    res.json({ characterId: playerId, skills: skills });
});

// [POST] 스킬 레벨업 저장
app.post('/api/skill/upgrade', (req, res) => {
    const { characterId, skillId, newLevel } = req.body;

    // 데이터 업데이트 로직 (간단 예시)
    if (userSkills[characterId]) {
        const skill = userSkills[characterId].find(s => s.skillId === skillId);
        if (skill) {
            skill.level = newLevel;
            console.log(`${characterId}의 ${skillId}가 레벨 ${newLevel}로 상승!`);
            return res.status(200).json({ message: "Success", currentLevel: skill.level });
        }
    }
    res.status(404).json({ message: "Character or Skill not found" });
});

app.listen(port, () => {
    console.log(`Server is running at http://localhost:${port}`);
});