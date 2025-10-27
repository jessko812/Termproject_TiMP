import struct

with open('vectors.bin', 'wb') as f:
    # Количество векторов: 2 (uint32_t)
    f.write(struct.pack('<I', 2))
    
    # Первый вектор: размер 3 (uint32_t), значения [1, 2, 3] (int16_t)
    f.write(struct.pack('<I', 3))
    f.write(struct.pack('<3h', 1, 2, 3))
    
    # Второй вектор: размер 2 (uint32_t), значения [4, 5] (int16_t)
    f.write(struct.pack('<I', 2))
    f.write(struct.pack('<2h', 4, 5))

print("Создан тестовый файл vectors.bin")
