## REST API

### Usage

This API is used by the daemon with the others client to communicate.

It is used above an HTTP connection.

### History

#### Version 1.2

* Add the List command
* Add the Validate command
* Add the File_From_Id command

#### Version 1.1

* Add the force parameter in the Analyze request.

#### Version 1

* API created.
* Command: Analyze, Status, Report, Retry, Clear

### API

Current API version: $API_VERSION = 1.2

#### Command

* Analyze:      HTTP POST
* Status:       HTTP GET
* Report:       HTTP POST
* Retry:        HTTP PUT
* Clear:        HTTP DELETE
* List:         HTTP GET
* Validate:     HTTP POST
* File_From_Id: HTTP POST

#### Analyze

JSON format for the parameters.
URL: /$API_VERSION/analyze

##### Request

Parameters:

* args:              Array of arguments

- file:              String: Name of the file to analyze
- id:                Integer: a unique id for the request
- force:             Boolean: force to analyze the file even if registered in database (introduced in v1.1)

##### Response

Parameters:

* ok:                Array of valid arguments

- inId:              Integer: id given by the request
- outId:             Integer: unique id generated by the daemon
- create:            Boolean: if the file was not registered

* nok: Array of invalid arguments

- id:                Integer: id given by the request
- error:             Integer: Error code corresponding to the error

#### Status

URI format for the parameters.
URL: /$API_VERSION/status

##### Request

Parameters:

* List of:

- id:                Integer: id given by the Analyze command

##### Response

Parameters:

* ok:                Array of valid arguments

- id:                Integer: id given by the request
- finish:            Boolean: if the file is finished to be analyzed
- done:              Double: Percent done by the analysis

* nok:               Array of invalid arguments

- id:                Integer: id given by the request
- error:             Integer: Error code corresponding to the error

#### Report

JSON format for the parameters.
URL: /$API_VERSION/report

##### Request

Parameters:

* args:              Array of arguments

- id:                Integer: id given by the Analyze command
- reports:           Array of report kinds
- policies_contents: Array of policies contents (Strings)
- policies_names:    Array of policies names (Strings)
- display_content:   String:  display content
- display_name:      String:  display name (HTML, TEXT, XML, MAXML, JSTREE)

##### Response

Parameters:

* ok:                Structure of the following parameters

- report:            String: Contain the report corresponding to all requested parameters
- valid:             Boolean: true if all policies are valid

* nok:               Array of invalid arguments

- id:                Integer: id given by the request
- error:             Integer: Error code corresponding to the error

#### Retry

JSON format for the parameters.
URL: /$API_VERSION/retry

##### Request

Parameters:

* ids:              Array of id given by the Analyze command

##### Response

Parameters:

* ok:                Array of Integer: ids of the files analyzed again

* nok:               Array of invalid arguments

- id:                Integer: id given by the request
- error:             Integer: Error code corresponding to the error

#### Clear

URI format for the parameters.
URL: /$API_VERSION/clear

##### Request

Parameters:

* List of:

- id:                Integer: id given by the Analyze command

##### Response

Parameters:

* ok:                Array of Integer: ids of the files removed from Database

* nok:               Array of invalid arguments

- id:                Integer: id given by the request
- error:             Integer: Error code corresponding to the error

#### List

No Parameter

##### Request

Parameters:

##### Response

Parameters:

* files:             Array of files registered

- file:              Filename of the file
- id:                Internal id, id to be used by other request

#### Validate

JSON format for the parameters.
URL: /$API_VERSION/validate

##### Request

Parameters:

* args:              Array of arguments

- id:                Integer: id given by the Analyze command
- report:            report kind, only POLICY or IMPLEMENTATION are possible
- policies_contents: Array of policies contents (Strings)
- policies_names:    Array of policies names (Strings)

##### Response

Parameters:

* ok:                Structure of the following parameters

- id:                Integer: Id given by the request
- valid:             Boolean: true if all policies are valid for this file

* nok:               Array of invalid arguments

- id:                Integer: id given by the request
- error:             Integer: Error code corresponding to the error

#### File_From_Id

JSON format for the parameters.
URL: /$API_VERSION/file_from_id

##### Request

Parameters:

- id:                Integer: id given by the Analyze command

##### Response

Parameter:

* file:              String: File corresponding to the id, left empty if id not matching

#### Report kinds

* NO\REPORT:      0
* POLICY:         1
* IMPLEMENTATION: 2
* MEDIAINFO:      3
* MEDIATRACE:     4

#### Error reason

* NOT\_REASON           0
* FILE\_NOT\_EXISTING   1
* ID\_NOT\_EXISTING     2
* NOT\_READY            3