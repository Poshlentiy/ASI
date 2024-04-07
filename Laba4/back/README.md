# Инструкция к запуску

1. Создание виртуальной среды Python в котором устанавливаются зависимости и библиотеки (python -m venv venv)
2. Активация виртуальной среды (venv\Scripts\activate.bat)
3. Установка всех необходимых библиотек в виртуальную среду (pip install -r requirements.txt). В текстовом файле requirements.txt прописаны заголовки и версии всех библиотек, используемых в проекте.
4. Создание Docker-контейнера с образом MongoDB (docker run --name mongodb -d -p 27017:27017 mongodb/mongodb-community-server)
5. Запуск сервера (python main.py)
