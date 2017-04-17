cbit
====

_cucm bulk import tool_

# What do you need?

* [Cisco Unified Communication Manager](http://www.cisco.com/c/en/us/products/unified-communications/unified-communications-manager-callmanager/index.html)
* [PHP](http://php.net) with [cURL](http://curl.haxx.se) support
* Administrative XML API documentation from [here](https://developer.cisco.com/site/axl/develop-and-test/documentation/previous-versions/) or [here](https://developer.cisco.com/site/axl/develop-and-test/documentation/latest-version/), depending on your Cisco Unified Communication Manager version

**DISCLAIMER:** It's assumed you have admin access to your Cisco Unified Communication Manager, are familiar with the GUI and that you generally know what you are doing. Depending on your environment and for security reasons, creating a special user for the script that can only access the Administrative XML API, is recommended.

# What is it?

The _cucm bulk import tool_ (_cbit_), is a cli script written in PHP to automate adding/deleting/updating objects (for example: srst references, device pools, sip trunks, locations, hunt pilots, phones...) on the Cisco Unified Communication Manager (CUCM) using it's Administrative XML (AXL) API.

# What can you use this for?

Usually a site on the CUCM consists of several objects. Some are mandatory, some are optional depending on your overall voice network design and features you have in use. A site maybe needs a location, srst reference, calling search space, possibly a region, site specific partitions, translation patterns, phones, a gateway and likely some more objects, which you need to create for every site over and over again. Or you are working in a lab environment and need to create/delete the above over and over again to test and simulate different setups.

In large installations chances are high that the settings for most of the above mentioned objects look the same per site - just the name is different i.e. __SiteA\_SRST__, __SiteB\_SRST__ or __SiteA\_Xlate\_PT__, __SiteB\_Xlate\_PT__ so on and so forth.

With _cbit_ you can write yourself templates for all the different objects which look the same and a csv file with all variable data that changes per site. Then you let _cbit_ do the work: "Merging" the templates with the data from the csv file and then adding the objects to the CUCM. It does not matter anymore if you need to add one partition or a hundred.

# How does it work?

First, let's have a look at the example files included:

## example_addSrst1.xml
This is an example to add a srst reference to the CUCM:

```
<soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:ns="http://www.cisco.com/AXL/API/9.1">
<soapenv:Header/>
	<soapenv:Body>
		<ns:addSrst>
			<srst>
				<name>[[cbitId]]_SRST</name>
				<ipAddress>[[cbitSrst1Ip]]</ipAddress>
				<SipNetwork>[[cbitSrst1Ip]]</SipNetwork>
			</srst>
		</ns:addSrst>
	</soapenv:Body>
</soapenv:Envelope>
```

## example_addRoutePartition1.xml
This is an example to add a route partition to the CUCM:

```
<soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:ns="http://www.cisco.com/AXL/API/9.1">
<soapenv:Header/>
	<soapenv:Body>
		<ns:addRoutePartition>
			<routePartition>
				<name>[[cbitId]]_Xlate_PT</name>
				<description>[[cbitId]] Site Specific Partition</description>
			</routePartition>
		</ns:addRoutePartition>
	</soapenv:Body>
</soapenv:Envelope>
```

## example.csv
This is an example csv where the variable data comes from:

```
cbitId;cbitSrst1Ip
SiteA;10.10.10.1
SiteB;10.10.20.1
SiteC;10.10.30.1
```

**IMPORTANT**: By default, _cbit_ uses `;` (semicolon) as delimiter for the csv files. This can be changed to `,` (comma) in the config file `cbit_config` if necessary. Other delimiters are not supported!

---

Now let's look a little closer at the first template. The first line always has to contain the AXL API version of the CUCM _cbit_ talks to. The important part is within the name space declaration: `xmlns:ns="http://www.cisco.com/AXL/API/9.1"` In this case we instruct to use AXL API verion **9.1**. _cbit_ will read this information and will check if the version of the CUCM it talks to supports this AXL API version. It will also tell you what older AXL API versions you can use with your version of CUCM (usefull in case you want to reuse older templates after you did upgrade your CUCM).

The rest of the SOAP message are the instructions to create a srst reference on CUCM (in v9.1 of the AXL API). You can find the details on how to compose these SOAP messages in the AXL API documentation. You can download the documentation [here](https://developer.cisco.com/site/axl/develop-and-test/documentation/previous-versions/) or [here](https://developer.cisco.com/site/axl/develop-and-test/documentation/latest-version/), depending on your CUCM version.

The second template follows the same structure. In this case it's a SOAP message to create a route partition.

## Variables

Maybe you did spot the words in double square brackets `[[` and `]]` in the templates. These are actually **variables** which make the connection to the csv file. There are a couple rules when it comes to these variables:

1. Only the following characters are allowed: **a-z** **A-Z** **0-9** **_** (underscore) **-** (dash/hyphen)
2. Variable names are **CaSe sEnSiTiVe**
3. Obviously there are **no duplicate variables** allowed in the csv header

**IMPORTANT:** There is one **mandatory variable** that needs to be in every csv file: `cbitId`. This one is used internally by _cbit_ to display progress and error reports. You can populate this variable how you want, but a name that represents the dataset of the given csv line is recommended. In the example we use site names (SiteA, SiteB & SiteC). Depending on the type of the job you run, other examples might be userids, DNs or if you can't think of anything just use a running number. You are not required to use **cbitId** within your templates (since it's only mandatory to be in the csv file) but you can if you want to. In the example it is used in the templates to show the recommended practice.

In the csv file you have a header in the first line, where you can find the same **variables** just without the brackets. In this case we have `cbitId;cbitSrst1Ip` followed by the actual data for three sites. The identifier of the site and the ip used for the srst reference, one site per line.

---

When _cbit_ is run, it takes the templates and merges them with the csv data like so:

1. It will take the first template in the chain `example_addSrst1.xml`, replace the **variables** within the template with the data from the csv file and submit the so compsed SOAP message to the CUCM for processing. It will repeat this for any line found in the csv file.
2. When done _cbit_ will take the second template `example_addRoutePartition1.xml` and do the same thing.
3. This repeats until no further template is found in the chain.

When finished, _cbit_ created three srst references:

1. SiteA\_SRST with IP 10.10.10.1
2. SiteB\_SRST with IP 10.10.20.1
3. SiteC\_SRST with IP 10.10.30.1

And three route partitions:

1. SiteA\_Xlate\_PT
2. SiteB\_Xlate\_PT
3. SiteC\_Xlate\_PT

That way you can chain as many different templates as you would like and merge these with as many csv data as you would like to build complete sites on the CUCM.

# Usage

## Configuration

_cbit_ uses a config file called `cbit_config` which needs to be placed in the same directory as the main script. There are three main parts that you need to configure before you can use _cbit_.

1. You need to configure at least one profile which describes how your CUCM is reached. You can configure more than one if you need to:

 ```
//cluster information
$cucm_profile[0]['url'] = 'https://x.x.x.x:8443/axl/';
$cucm_profile[0]['user'] = 'user';
$cucm_profile[0]['pass'] = 'pass';
$cucm_profile[0]['interface'] = 'interface';
$cucm_profile[0]['description'] = 'my cucm';
```

 The values `url`, `user`, `pass` and `description` are pretty self explanatory. Set the `interface` value to the outgoing interface of the machine you run _cbit_ on - i.e. eth0. You can configure _cbit_ to use any interface or subinterface in case your machine has multiple.

2. You then pick the desired profile. In our example this is `0` as we only have one:

 ```
//select cucm profile to use
$cucm = 0;
```

3. Last but not least, you enter all templates you did create. This basically builds a chain of templates that will be worked top to bottom. Since you can't add let's say a partition to a calling search space that does not exist, you need to make sure you put the template that creates the partition **before** the template that creates the calling search space - because they depend on each other. So order is important!

 ```
//configured templates (check order and dependency!)
$templates[] = 'example_addSrst1.xml';
$templates[] = 'example_addRoutePartition1.xml';
```

You usually don't need to tamper with the rest of the `cbit_config` file.

## Running the script

Once you have completed all your templates and your `cbit_config` file you can run _cbit_.

### Command line options

* There is a `-h` option which displays a little help like so:

 ```
$ ./cbit -h
20:00:00 cbit r24 starting up
20:00:00 use -h for help
20:00:00 using cbit.log to log messages
20:00:00 checking for required modules
20:00:00   curl [ok]
20:00:00 available command line options:
20:00:00
20:00:00 -h display this help
20:00:00
20:00:00 -c dump config
20:00:00 -b<iterations> bench latency (defaults to 10 if <iterations> is omitted)
20:00:00
20:00:00 -f<file> specify source csv file
20:00:00 -s disable skip mode (skip mode is enabled by default)
20:00:00
20:00:00 The MIT License (MIT) - see LICENSE file
20:00:00
20:00:00 cbit r24 Copyright (c) 2014-2017 Stephan Eisfeld
20:00:00
20:00:00 exiting
```

* The `-c` option dumps the most important part of the config to the terminal. The CUCM server configured, outgoing interface and the configured template chain including their AXL API versions. _cbit_ will also tell you if it actually found the template files on disk (they also need to be in the same directory as the script itself): It displays `[found]` or not `[not found]` respectively.

 ```
$ ./cbit -c
20:00:00 cbit r24 starting up
20:00:00 use -h for help
20:00:00 using cbit.log to log messages
20:00:00 checking for required modules
20:00:00   curl [ok]
20:00:00 dumping config
20:00:00 user:pass@https://x.x.x.x:8443/axl/ (my cucm)
20:00:00 outgoing interface: interface
20:00:00 csv delimiter: ;
20:00:00 2 template(s) configured
20:00:00   example_addSrst1.xml | 9.1 [found]
20:00:00   example_addRoutePartition1.xml | 9.1 [found]
20:00:00 exiting
```

* The `-f` option requires an additional argument, `<file>`. This is to pass the csv file to _cbit_. Doing so will give you something like the following:

 ```
$ ./cbit -fexample.csv
20:00:00 cbit r24 starting up
20:00:00 use -h for help
20:00:00 using cbit.log to log messages
20:00:00 checking for required modules
20:00:00   curl [ok]
20:00:00 using example.csv as source [ok]
20:00:00   checking csv delimiter [ok]
20:00:00     configured: ;
20:00:00       detected: ;
20:00:00   checking for valid header [2 columns]
20:00:00   checking for cbitId in header [ok]
20:00:00   checking for duplicates in header [ok]
20:00:00   removing empty data rows [0 removed]
20:00:00   checking data column count vs header count [ok]
20:00:00   checking for empty cells in data rows [ok]
20:00:00   number of data rows to process [3]
20:00:00 skip mode enabled (default, use -s to disable)
20:00:00 ----------------------------------------
20:00:00 checking https://x.x.x.x:8443/axl/ (my cucm) reachablility [ok]
20:00:00 checking if axl api is responding and supported [ok]
20:00:00   connect time: 5ms | total request time: 50ms
20:00:00   found cucm version: 9.1.2.45642
20:00:00     assuming cucm:db: 9.1 [supported]
20:00:00 looking for configured template(s) [2 configured]
20:00:00 checking for supported api version in template(s):
20:00:00   cucm:db 9.1 supports apis: 9.1, 9.0, 8.5, 8.0, 7.1
20:00:00     example_addSrst1.xml | 9.1 [hit]
20:00:00     example_addRoutePartition1.xml | 9.1 [hit]
20:00:00 checking templates for unknown variables [ok]
20:00:00 cbit is ready to start. ok to proceed? (y/[n]): y
20:00:00 ----------------------------------------
20:00:00 starting work
20:00:00 working on template: example_addSrst1.xml
20:00:00 0001 SiteA [done] 1ms/175ms | {13FF0BAC-58A0-75BF-DD99-63F9B27DD27B}
20:00:00 0002 SiteB [done] 1ms/191ms | {F255E4E3-7818-B0C9-7A3F-3AD7F8CCE056}
20:00:00 0003 SiteC [done] 1ms/232ms | {2B79EDE2-2FDC-4EF4-5885-B767510B4381}
20:00:00 working on template: example_addRoutePartition1.xml
20:00:00 0001 SiteA [done] 1ms/195ms | {BD311245-5973-A82F-1FDC-7D5E23DFBDB6}
20:00:00 0002 SiteB [done] 1ms/169ms | {968B2EAD-1693-CB89-198C-5C9529ECCAF5}
20:00:00 0003 SiteC [done] 1ms/142ms | {496E5363-02F0-A17F-F07B-9B1BCD26A5FC}
20:00:00 ----------------------------------------
20:00:00 0 job(s) with errors/exceptions
20:00:00 ----------------------------------------
20:00:00 execution time: 0m 4s
20:00:00 renaming cbit.log -> example.1388602800.log
20:00:00 exit
```

 We passed `-fexample.csv` which tells _cbit_ to use `example.csv` as source. It then does various checks like: Does the source file actually exist (again, needs to be in the same directory as the script itself), a couple sanity checks with the source file and a reachability check of the cucm server.

 Once that is passed, it immediately checks if the AXL API is responding or not. Make sure the user you supplied in your `cbit_config` has sufficient rights to use CUCM's AXL API and that the AXL API related services are actually running. These are:
 * Cisco Bulk Provisioning Service
 * Cisco AXL Web Service

 It is recommended to create a separate user for _cbit_ which can only access the AXL API but nothing else.

 If the AXL API is responding _cbit_ pulls the version of the CUCM and derives the cucm:db version from that. It then checks if it has support for that cucm:db version. If yes, it checks all templates for the AXL API version specified and compares these to a list of AXL API versions supported by the cucm:db version discovered before:

 ```
20:00:00   found cucm version: 9.1.2.45642
20:00:00     assuming cucm:db: 9.1 [supported]
20:00:00 looking for configured template(s) [2 configured]
20:00:00 checking for supported api version in template(s):
20:00:00   cucm:db 9.1 supports apis: 9.1, 9.0, 8.5, 8.0, 7.1
20:00:00     example_addSrst1.xml | 9.1 [hit]
20:00:00     example_addRoutePartition1.xml | 9.1 [hit]
```

 In the above output all was okay - all templates had a `[hit]`. It would display `[miss]` if the version set in the templates is not supported by the cucm:db discovered before.

 Then _cbit_ compares all variables it found in the templates with the ones in the csv file. This is to ensure we have a matchup between the two:

 ```
20:00:00 checking templates for unknown variables [ok]
```

 If every check passed successfully up to this point, _cbit_ asks the user if he is ok to proceed with the actual work:

 ```
20:00:00 cbit is ready to start. ok to proceed? (y/[n]): y
20:00:00 ----------------------------------------
20:00:00 starting work
20:00:00 working on template: example_addSrst1.xml
20:00:00 0001 SiteA [done] 1ms/175ms | {13FF0BAC-58A0-75BF-DD99-63F9B27DD27B}
20:00:00 0002 SiteB [done] 1ms/191ms | {F255E4E3-7818-B0C9-7A3F-3AD7F8CCE056}
20:00:00 0003 SiteC [done] 1ms/232ms | {2B79EDE2-2FDC-4EF4-5885-B767510B4381}
20:00:00 working on template: example_addRoutePartition1.xml
20:00:00 0001 SiteA [done] 1ms/195ms | {BD311245-5973-A82F-1FDC-7D5E23DFBDB6}
20:00:00 0002 SiteB [done] 1ms/169ms | {968B2EAD-1693-CB89-198C-5C9529ECCAF5}
20:00:00 0003 SiteC [done] 1ms/142ms | {496E5363-02F0-A17F-F07B-9B1BCD26A5FC}
20:00:00 ----------------------------------------
20:00:00 0 job(s) with errors/exceptions
20:00:00 ----------------------------------------
20:00:00 execution time: 0m 4s
20:00:00 renaming cbit.log -> example.1388602800.log
20:00:00 exit
```

 Three srst references have been created as well as three route partitions. The UUIDs show the success. In case of a failure you would get an error message from the AXL API displayed there.

* The `-s` option disables what is called "skip mode" which is enabled by default. Skip mode ensures that _cbit_ does not try to process further templates for a site, if a previous template failed for whatever reason. This is to ensure subsequent templates don't fail due to missing objects that failed to create before. Like you can't add a partition to a calling search space if it failed to create in the first place. By disabling skip mode you can force _cbit_ to process all templates even if a previous one failed. If skip mode is disabled you will see `skip mode disabled via command line (-s)` in the output.

* The `-b` option takes an optional argument `<iterations>`. Passing the `-b` option to _cbit_ runs a very simple latency test against the AXL API. If `<iterations>` are not specified, it defaults to 10 iterations. The output looks like this:

 ```
$ ./cbit -b5
20:00:00 cbit r24 starting up
20:00:00 use -h for help
20:00:00 using cbit.log to log messages
20:00:00 checking for required modules
20:00:00   curl [ok]
20:00:00 checking https://x.x.x.x:8443/axl/ (my cucm) reachablility [ok]
20:00:00 checking if axl api is responding and supported [ok]
20:00:00   connect time: 5ms | total request time: 50ms
20:00:00   found cucm version: 9.1.2.45642
20:00:00     assuming cucm:db: 9.1 [supported]
20:00:00 benching latency with 5 iterations
20:00:00   connect time: 1ms, total request time: 135ms
20:00:00   connect time: 1ms, total request time: 131ms
20:00:00   connect time: 1ms, total request time: 168ms
20:00:00   connect time: 1ms, total request time: 140ms
20:00:00   connect time: 1ms, total request time: 159ms
20:00:00   average connect time: 1ms
20:00:00   average total request time: 147ms
20:00:00 exiting
```

## The log file

_cbit_ writes a quite verbose log file everytime you run it. The default name is `cbit.log` but this can be changed in the config file `cbit_config`:

```
//use the following file as log file
$log_file = 'cbit.log';
```

Normally the log file is overwritten everytime you run _cbit_ as long as you don't use the `-f` option. This is because the log file then contains every single SOAP request sent to the CUCM as well as the answer (in raw format) from the AXL API for your reference. To conserve this information the log file is renamed once _cbit_ finished up:

```
renaming cbit.log -> example.1388602800.log
```

The filename is derived from the csv file name - given to `-f` - and the current unix time stamp to quickly build a rather unique name.

# The MIT License (MIT)

**Copyright (c) 2014-2017 Stephan Eisfeld**

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

**THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.**
