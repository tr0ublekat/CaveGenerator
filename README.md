# Генерация пещер при помощи клеточного автомата

## Запуск

### Windows 
Запустить файл WindowsVisualStudio/WindowsVisualStudio.sln (Visual Studio, библиотеки уже установлены локально в проекте).
### Linux
Установить библиотеки: sudo apt-get install libgl1-mesa-dev freeglut3-dev libglu1-mesa-dev
Запустить скрипт start.sh.

## Горячие клавиши
-1-6 – выбор размера поля
-WASD – передвижение камеры
-‘-‘, ‘+’ – приближение камеры
-N – пересоздание поля
-‘\’ – сохранение пещеры в .bmp
-Space – следующая итерация
-‘[‘ и ‘]’ – установить правила (B5678/S45678 и B3/S23)
-‘Z’ и ‘X’ – уменьшение и увеличение процента первого поколения
-С – откат на одну итерацию
