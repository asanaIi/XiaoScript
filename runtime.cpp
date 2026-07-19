#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cstdlib>

std::string trimSpace(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Удаление кастомных комментариев вида <! комментарий !>
std::string removeXiaoComments(std::string content) {
    size_t start_pos;
    while ((start_pos = content.find("<!")) != std::string::npos) {
        size_t end_pos = content.find("!>", start_pos);
        if (end_pos != std::string::npos) {
            content.erase(start_pos, (end_pos + 2) - start_pos);
        } else {
            break;
        }
    }
    return content;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Ошибка: Укажите файл сценария. Пример: xiaoscript main.xiao" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Ошибка: Не удалось открыть файл " << argv[1] << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    std::string raw_content = buffer.str();
    std::string clean_content = removeXiaoComments(raw_content);

    // Сбор переменных из тегов <Variable>
    std::map<std::string, std::string> variables;
    size_t var_pos = 0;
    while ((var_pos = clean_content.find("<Variable", var_pos)) != std::string::npos) {
        size_t end_tag = clean_content.find("/>", var_pos);
        if (end_tag == std::string::npos) break;

        std::string tag_body = clean_content.substr(var_pos, end_tag - var_pos);
        
        auto get_attr = [&](std::string attr) {
            size_t attr_pos = tag_body.find(attr + "=\"");
            if (attr_pos == std::string::npos) return std::string("");
            size_t start_val = attr_pos + attr.length() + 2;
            size_t end_val = tag_body.find("\"", start_val);
            return tag_body.substr(start_val, end_val - start_val);
        };

        std::string name = get_attr("name");
        std::string value = get_attr("value");
        if (!name.empty()) {
            variables[name] = value;
        }
        var_pos = end_tag + 2;
    }

    // Поиск игрового окна/логики внутри тега <App>
    size_t app_start = clean_content.find("<App>");
    size_t app_end = clean_content.find("</App>");
    
    if (app_start == std::string::npos || app_end == std::string::npos) {
        std::cerr << "Ошибка: Блок <App></App> не найден!" << std::endl;
        return 1;
    }

    app_start += 5; // Смещение за предел тега <App>
    std::string js_code = clean_content.substr(app_start, app_end - app_start);

    // Генерируем финальный JS файл, внедряя переменные из XML наверх скрипта
    std::ofstream temp_js("xiao_runtime_build.js");
    temp_js << "const HyperOS = {\n";
    for (auto const& [name, val] : variables) {
        temp_js << "  " << name << ": " << (val == "true" || val == "false" || val.find_first_not_of("0123456789") == std::string::npos ? val : "\"" + val + "\"") << ",\n";
    }
    temp_js << "};\n\n";
    temp_js << js_code;
    temp_js.close();

    // Запуск через Node.js окружение
    int result = std::system("node xiao_runtime_build.js");
    std::remove("xiao_runtime_build.js");

    return result;
}