import xml.etree.ElementTree as ET
import sys

def parse_manifest():
    try:
        tree = ET.parse('device_manifest.xml')
        root = tree.getroot()
        
        for var in root.findall('.//Variable'):
            var_type = var.get('type')
            name = var.get('name')
            value = var.get('value')
            # Выводим в формате, который легко прочитает Си-движок
            print(f"{var_type}:{name}:{value}")
            
    except Exception as e:
        sys.exit(1)

if __name__ == "__main__":
    parse_manifest()
