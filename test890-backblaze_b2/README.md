Gateway fails. Data seems to be pushed to the bucket but many error messages
are logged:

```
Oct 27 16:30:43 vagrant minio[21473]: API: PutObject(bucket=XXX, object=test/wal_005/000000010000000000000001.lz4)
Oct 27 16:30:43 vagrant minio[21473]: Time: 16:30:43 UTC 10/27/2019
Oct 27 16:30:43 vagrant minio[21473]: DeploymentID: XXX
Oct 27 16:30:43 vagrant minio[21473]: RequestID: XXX
Oct 27 16:30:43 vagrant minio[21473]: RemoteHost: ::1
Oct 27 16:30:43 vagrant minio[21473]: Host: localhost:9000
Oct 27 16:30:43 vagrant minio[21473]: UserAgent: aws-sdk-go/1.23.6 (go1.11.13; linux; amd64) S3Manager
Oct 27 16:30:43 vagrant minio[21473]: Error: b2 error: write tcp 10.0.2.15:48728->206.190.210.135:443: use of closed network connection
Oct 27 16:30:43 vagrant minio[21473]:        4: cmd/api-errors.go:1706:cmd.toAPIErrorCode()
Oct 27 16:30:43 vagrant minio[21473]:        3: cmd/api-errors.go:1731:cmd.toAPIError()
Oct 27 16:30:43 vagrant minio[21473]:        2: cmd/object-handlers.go:1259:cmd.objectAPIHandlers.PutObjectHandler()
Oct 27 16:30:43 vagrant minio[21473]:        1: net/http/server.go:2007:http.HandlerFunc.ServeHTTP()
```
