import sys
from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.options import Options

if len(sys.argv) < 2:
    print("Use: python scrape.py <URL>")
    sys.exit(1)

url = sys.argv[1]

if len(sys.argv) == 3:
    path = sys.argv[2]
    service = webdriver.ChromeService(executable_path = path)
else:
    service = webdriver.ChromeService()

# Headless mode, not working
#chrome_options = Options()
#chrome_options.add_argument("--headless")
#driver = webdriver.Chrome(service=service, options=chrome_options)

driver = webdriver.Chrome()

driver.get(url)

html = driver.page_source

print(html)

driver.quit()

