.PHONY: clean

apply.done: init.done
	terraform apply
	touch $@

init.done:
	terraform init
	touch $@

clean:
	terraform destroy
	rm -f init.done apply.done
