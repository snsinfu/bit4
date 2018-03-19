TEST_NAME = lambda

.PHONY: test clean

test: request.done
	@  ACTUAL="$$(cat $<)"              \
	&& EXPECT="Hello, $(TEST_NAME)!"    \
	&& test "$${ACTUAL}" = "$${EXPECT}" \
	&& echo "OK"                        \
	|| { echo "Actual: '$${ACTUAL}'";   \
	     echo "Expect: '$${EXPECT}'"; }

clean:
	terraform destroy
	rm -f init.done deploy.done request.done hello.zip hello

init.done:
	terraform init
	touch $@

deploy.done: init.done main.tf hello.zip
	terraform apply
	touch $@

request.done: deploy.done
	curl -fsSL -D - -o $@ "$$(terraform output url)?name=$(TEST_NAME)"

hello.zip: hello
	zip $@ $<

hello: main.go
	go get .
	GOOS=linux GOARCH=amd64 go build -o $@
