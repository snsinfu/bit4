HD_SIZE = 20000
UUID = $$(cat box.uuid)
CHECKPOINTS = box-vm.ok box-cpu.ok box-hdd.ok box-iso.ok box-net.ok

.PHONY: start clean

start: box-vm.ok box-cpu.ok box-hdd.ok box-iso.ok box-net.ok
	VBoxManage startvm $(UUID) --type headless

clean:
	VBoxManage unregistervm $(UUID) --delete
	rm $(CHECKPOINTS) box.uuid

box.vdi: box.uuid
	VBoxManage createmedium disk --filename $@ --size $(HD_SIZE)

box.uuid:
	uuidgen > $@

centos.iso:
	curl -fSL -o $@ "http://ftp.iij.ad.jp/pub/linux/centos/7/isos/x86_64/CentOS-7-x86_64-Minimal-1804.iso"

box-cpu.ok: box-vm.ok
	VBoxManage modifyvm $(UUID) --cpus 2 --memory 1024
	touch $@

box-hdd.ok: box-vm.ok box.vdi box.uuid
	VBoxManage storagectl $(UUID) --name SATA --add sata --controller IntelAHCI
	VBoxManage storageattach $(UUID) --storagectl SATA --port 0 --device 0 --type hdd --medium box.vdi
	touch $@

box-iso.ok: box-vm.ok box.uuid centos.iso
	VBoxManage storagectl $(UUID) --name IDE --add ide
	VBoxManage storageattach $(UUID) --storagectl IDE --port 0 --device 0 --type dvddrive --medium centos.iso
	touch $@

box-net.ok: box-vm.ok
	VBoxManage modifyvm $(UUID) --nic1 nat
	touch $@

box-vm.ok: box.uuid
	VBoxManage createvm --basefolder $${PWD} --name box --uuid $(UUID) --register
	touch $@
