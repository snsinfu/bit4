Configure your AWS access key and secret key with `aws configure`, or just create a file `~/.aws/credentials` containing the keys:

```
[default]
aws_access_key_id = KEY
aws_secret_access_key = KEY
```

Then, set environment variable `AWS_DEFAULT_REGION` to your favorite region and run `make`. Terraform will ask you for a confirmation, so type yes. Then a Lambda function will be deployed with an API gateway.

Run `make clean` to release the Lambda/API gateway resources.
