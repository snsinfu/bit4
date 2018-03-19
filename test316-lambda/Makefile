.PHONY: test clean

init.done:
	terraform init
	touch $@

plan.done: init.done main.tf hello.zip
	terraform plan
	touch $@

deploy.done: init.done main.tf hello.zip
	terraform apply
	touch $@

request.done: deploy.done
	@  API_ID=$$(terraform state show aws_api_gateway_rest_api.hello | \
	             awk 'NR == 1 { print $$3 }')                          \
	&& REGION=$$(terraform state show aws_region.current             | \
	             awk 'NR == 1 { print $$3 }')                          \
	&& curl -fsSL -D - -o $@                                           \
	        "https://$${API_ID}.execute-api.$${REGION}.amazonaws.com/test/hello?name=lambda"

test: request.done
	@  ACTUAL="$$(cat $<)"              \
	&& EXPECT="Hello, lambda!"          \
	&& test "$${ACTUAL}" = "$${EXPECT}" \
	&& echo "OK"                        \
	|| { echo "Actual: '$${ACTUAL}'";   \
	     echo "Expect: '$${EXPECT}'"; }

clean:
	terraform destroy
	rm -f hello hello.zip init.done plan.done deploy.done request.done

hello.zip: hello
	zip $@ $<

hello: main.go
	GOOS=linux GOARCH=amd64 go build -o $@
