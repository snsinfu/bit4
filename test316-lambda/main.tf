data "aws_region" "current" {
}

# Lambda
resource "aws_lambda_function" "hello" {
  filename         = "hello.zip"
  function_name    = "hello"
  handler          = "hello"
  source_code_hash = "${base64sha256(file("hello.zip"))}"
  role             = "${aws_iam_role.hello.arn}"
  runtime          = "go1.x"
  memory_size      = 128
  timeout          = 1
}

resource "aws_iam_role" "hello" {
  name               = "hello"
  assume_role_policy = <<POLICY
{
  "Version": "2012-10-17",
  "Statement": {
    "Action": "sts:AssumeRole",
    "Principal": {
      "Service": "lambda.amazonaws.com"
    },
    "Effect": "Allow"
  }
}
POLICY
}

resource "aws_lambda_permission" "hello" {
  statement_id  = "AllowAPIGatewayInvoke"
  action        = "lambda:InvokeFunction"
  function_name = "${aws_lambda_function.hello.arn}"
  principal     = "apigateway.amazonaws.com"
}

# API Gateway
resource "aws_api_gateway_resource" "hello" {
  rest_api_id = "${aws_api_gateway_rest_api.hello.id}"
  parent_id   = "${aws_api_gateway_rest_api.hello.root_resource_id}"
  path_part   = "hello"
}

resource "aws_api_gateway_rest_api" "hello" {
  name = "hello"
}

resource "aws_api_gateway_method" "hello" {
  rest_api_id   = "${aws_api_gateway_rest_api.hello.id}"
  resource_id   = "${aws_api_gateway_resource.hello.id}"
  http_method   = "GET"
	authorization = "NONE"
}

resource "aws_api_gateway_integration" "hello" {
  rest_api_id             = "${aws_api_gateway_rest_api.hello.id}"
  resource_id             = "${aws_api_gateway_resource.hello.id}"
  http_method             = "${aws_api_gateway_method.hello.http_method}"
  integration_http_method = "POST"
  type                    = "AWS_PROXY"
  uri                     = "arn:aws:apigateway:${data.aws_region.current.name}:lambda:path/2015-03-31/functions/${aws_lambda_function.hello.arn}/invocations"
}

resource "aws_api_gateway_deployment" "hello_test" {
  depends_on  = [
    "aws_api_gateway_integration.hello"
  ]
  rest_api_id = "${aws_api_gateway_rest_api.hello.id}"
  stage_name  = "test"
}
