## Resources

### General guidelines

- https://docs.aws.amazon.com/apigateway/latest/developerguide/integrating-api-with-aws-services-lambda.html
- https://www.terraform.io/docs/providers/aws/guides/serverless-with-aws-lambda-and-api-gateway.html

### AWS documentations

- https://docs.aws.amazon.com/lambda/latest/dg/welcome.html
- https://docs.aws.amazon.com/lambda/latest/dg/go-programming-model.html

- https://docs.aws.amazon.com/apigateway/latest/developerguide/welcome.html
- https://docs.aws.amazon.com/apigateway/latest/developerguide/set-up-lambda-integrations.html

### Terraform documentations

- https://www.terraform.io/docs/providers/aws/r/lambda_function.html
- https://www.terraform.io/docs/providers/aws/r/api_gateway_integration.html

## Notes

### The dates

The hardcoded dates in the terraform file are AWS API versions:

- 2012-10-17 for [IAM](https://docs.aws.amazon.com/IAM/latest/UserGuide/reference_policies_elements_version.html)
- 2015-03-31 for [Lambda](https://docs.aws.amazon.com/lambda/latest/dg/history.html)

These are AWS API versions.

### IAM role is required

This hello world function does not access to any AWS resources (such as S3), but anyway an IAM role needs to be specified. Here no access is granted for the hello role.

### Integration Proxy -> Lambda requrest

The request type is `AWS_PROXY` and the method is `POST`. These are fixed parameters.

### Lambda response needs to be a structure

The Lambda output must be in [an exact form][lambda-output-format]. For a Golang lambda function, [the return value is automatically marshaled][lambda-go-return], so the response must be a structure with the required fields:

```
type Response struct {
	StatusCode      int               `json:"statusCode"`
	Headers         map[string]string `json:"headers"`
	Body            string            `json:"body"`
	IsBase64Encoded bool              `json:"isBase64Encoded"`
}
```

[lambda-output-format]: https://docs.aws.amazon.com/apigateway/latest/developerguide/set-up-lambda-proxy-integrations.html#api-gateway-simple-proxy-for-lambda-output-format
[lambda-go-return]: https://docs.aws.amazon.com/lambda/latest/dg/go-programming-model-handler-types.html#go-programming-model-handler-types-signatures

### Lambda handler is the executable

For Golang lambda function the lambda handler is the name of the executable.

```
resource "aws_lambda_function" "hello" {
  ...
  handler = "hello"  # <-- The name of the executable
  ...
}
```
