(defrule ctr_l
 (test (IsBatteryTooLow "xxx"))
 =>
 (ChangeTimeReportLeader 50 "xxx")
)