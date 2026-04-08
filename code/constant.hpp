#pragma once
constexpr int windowWidth = 1600;
constexpr int windowHeight = 900;
constexpr int floorHeight = windowHeight * 10 / 12;

constexpr int playerSize = windowHeight * 20/600;
constexpr int rotationSpeed = 250;

constexpr float minSpawnTime = .75f;
constexpr float maxSpawnTime = 2.f;
constexpr float minSpawnTimeFlying = 1.5f;
constexpr float maxSpawnTimeFlying = 2.f;

constexpr float obstacleSpeed = windowWidth * 250.f / 1024;
constexpr float obstacleSpeedFlight = windowWidth * 300.f / 1024;
constexpr float groundRatio = windowHeight / 120;
constexpr float airRatio = windowHeight / 170;

constexpr float aFps = 1;

constexpr int charSize = windowHeight / 15;