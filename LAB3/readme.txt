�I�s�����G
	./monitorDHT.py [bcm_pin] ['condition'] [monit_time] ['callback']

�ѼơG
	bcm_pin: Ū����Ƥ��w�}�C

	monitor_time: �ʱ��ɶ��A�W�L������^�C

	condition: callback Ĳ�o����
		   ***�����ϥΤ��P����Ū�X���ƭȬ�t,h
		      ���O���ūפ���סA�䤤��פ��Ȭ� 0 ~ 1 ����


	callback: �ŦX����������檺���O
		  ���b�׺ݾ����Ҥ��i���檺���O�A�p
		  ls -alh
		  ./buzzer_play 2 freq_sequence.json (�@�ӥi�����ɮ�)
		  ***���ɮ׶ȯ���� 'ls' './test0.py' './test1.py' ���������O

�����G
	callback ��X���G�����O���� log.txt
	log.txt ���t  stdout / stderr/return value

