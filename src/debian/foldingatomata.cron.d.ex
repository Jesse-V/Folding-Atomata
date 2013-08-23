#
# Regular cron jobs for the foldingatomata package
#
0 4	* * *	root	[ -x /usr/bin/foldingatomata_maintenance ] && /usr/bin/foldingatomata_maintenance
