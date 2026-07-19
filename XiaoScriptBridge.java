import java.io.BufferedReader;
import java.io.InputStreamReader;

public class XiaoScriptBridge {
    public static void main(String[] args) {
        System.out.println("[Framework] Инициализация XiaoScript Runtime...");
        
        try {
            // Запуск скомпилированного на C++ ядра системы
            Process process = Runtime.getRuntime().exec("./runtime");
            
            BufferedReader reader = new BufferedReader(
                new InputStreamReader(process.getInputStream())
            );
            
            String line;
            while ((line = reader.readLine()) != null) {
                System.out.println("[DisplayServices] " + line);
            }
            
            process.waitFor();
        } catch (Exception e) {
            System.err.println("[Framework] Ошибка выполнения: " + e.getMessage());
        }
    }
}