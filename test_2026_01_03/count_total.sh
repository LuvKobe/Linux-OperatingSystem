#!/bin/bash
LOG_FILE="/edison/edison/csdn.log"
echo "========= CSDN 刷量战果统计 ========="
echo "运行起始时间: $(head -n 5 $LOG_FILE | grep -oP '\d{4}-\d{2}-\d{2}' | head -1 || echo '未知')"
echo "总成功次数: $(grep -c "成功访问" $LOG_FILE)"
echo "总失败(521): $(grep -c "521" $LOG_FILE)"
echo "-------------------------------------"
echo "文章具体访问分布 (次数 链接):"
grep "成功访问" $LOG_FILE | awk -F': ' '{print $2}' | sort | uniq -c | sort -nr
echo "====================================="
