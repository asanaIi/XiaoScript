const { execSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const args = process.argv.slice(2);
if (args.length === 0) {
    console.log("Использование: node XiaoScriptBridge2.js <filename.xiao>");
    process.exit(1);
}

const targetFile = args[0];

// Автоматическая компиляция монолита С++ при первом запуске или изменениях
if (!fs.existsSync('./runtime') && !fs.existsSync('./runtime.exe')) {
    console.log("[HyperOS] Компиляция ядра C++ движка...");
    try {
        execSync('g++ -std=c++17 runtime.cpp -o runtime');
    } catch (error) {
        console.error("Ошибка компиляции С++ ядра. Проверьте наличие g++.");
        process.exit(1);
    }
}

// Запуск скомпилированного С++ движка с передачей файла исходного кода
try {
    const cmd = process.platform === 'win32' ? `runtime.exe ${targetFile}` : `./runtime ${targetFile}`;
    execSync(cmd, { stdio: 'inherit' });
} catch (err) {
    process.exit(1);
}