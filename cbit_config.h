<?php
//cbit - cucm bulk import tool

//cluster information
$cucm_profile[0]['url'] = 'https://x.x.x.x:8443/axl/';
$cucm_profile[0]['user'] = 'user';
$cucm_profile[0]['pass'] = 'pass';
$cucm_profile[0]['interface'] = 'interface';
$cucm_profile[0]['description'] = 'my cucm';

//select cucm profile to use
$cucm = 0;

//configured templates (check order and dependency!)
$templates[] = 'example_addSrst1.xml';
$templates[] = 'example_addRoutePartition1.xml';

//******************** usually you don't have to change anything below this line ********************

//use the following file as temporary log file
$log_file = 'cbit.log';

//use the following character as csv delimiter
//only change if you know what you are doing!
$csv_delimiter=';';

//end
?>