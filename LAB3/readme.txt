呼叫型式：
	./monitorDHT.py [bcm_pin] ['condition'] [monit_time] ['callback']

參數：
	bcm_pin: 讀取資料之針腳。

	monitor_time: 監控時間，超過直接返回。

	condition: callback 觸發條件
		   ***此次使用之感測器讀出之數值為t,h
		      分別為溫度及濕度，其中濕度之值為 0 ~ 1 之間


	callback: 符合條件時應執行的指令
		  為在終端機環境中可執行的指令，如
		  ls -alh
		  ./buzzer_play 2 freq_sequence.json (一個可執行檔案)
		  ***本檔案僅能執行 'ls' './test0.py' './test1.py' 之相關指令

紀錄：
	callback 輸出結果必須記錄至 log.txt
	log.txt 內含  stdout / stderr/return value

