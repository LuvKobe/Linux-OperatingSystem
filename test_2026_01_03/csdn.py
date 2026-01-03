import cloudscraper
import random
import time
from concurrent.futures import ThreadPoolExecutor

# 模拟真实的浏览器请求头
headers = {
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36",
    "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8",
    "Accept-Language": "zh-CN,zh;q=0.9,en;q=0.8",
    "Referer": "https://www.google.com/",
}

Url = [
    'https://blog.csdn.net/m0_63325890/article/details/156539486',
    'https://blog.csdn.net/m0_63325890/article/details/156515461',
    'https://blog.csdn.net/m0_63325890/article/details/156430422',
    'https://blog.csdn.net/m0_63325890/article/details/156367240',
    'https://blog.csdn.net/m0_63325890/article/details/155982583',
    'https://blog.csdn.net/m0_63325890/article/details/155972150',
    'https://blog.csdn.net/m0_63325890/article/details/155608795',
    'https://blog.csdn.net/m0_63325890/article/details/155574567',
    'https://blog.csdn.net/m0_63325890/article/details/155571502',
    'https://blog.csdn.net/m0_63325890/article/details/154947231',
    'https://blog.csdn.net/m0_63325890/article/details/154061744',
    'https://blog.csdn.net/m0_63325890/article/details/155094250',
    'https://blog.csdn.net/m0_63325890/article/details/154683398',
    'https://blog.csdn.net/m0_63325890/article/details/154522388'
]

def brush_task(thread_id):
    # 为每个线程创建独立的 scraper 实例，应对各自的挑战
    scraper = cloudscraper.create_scraper(
        browser={
            'browser': 'chrome',
            'platform': 'windows',
            'desktop': True
        }
    )

    while True:
        target_url = random.choice(Url)
        try:
            # 执行请求
            response = scraper.get(target_url, timeout=20, headers=headers)

            if response.status_code == 200:
                print(f"[线程{thread_id}] 成功访问: {target_url}")
                # 1. 成功访问后，按照要求休息 15-30 秒
                wait = random.uniform(15, 30)
                print(f"[线程{thread_id}] 正常休息 {wait:.2f} 秒...")
                time.sleep(wait)
            elif response.status_code == 521:
                # 2. 遇到 521，该线程直接休息 1 分钟
                print(f"[线程{thread_id}] 检测到 521 拦截！触发冷静机制，强制休息 60 秒...")
                time.sleep(60)
            else:
                print(f"[线程{thread_id}] 异常状态码: {response.status_code}，休息 30 秒...")
                time.sleep(30)

        except Exception as e:
            print(f"[线程{thread_id}] 请求异常: {type(e).__name__}，10秒后重试...")
            time.sleep(10)

if __name__ == '__main__':
    max_threads = 5  # 确认使用 5 个线程
    print(f">>> 5 线程火力全开模式已启动 (15-30s 轮询 / 521 智能避让) <<<")

    with ThreadPoolExecutor(max_workers=max_threads) as executor:
        for i in range(max_threads):
            executor.submit(brush_task, i)
