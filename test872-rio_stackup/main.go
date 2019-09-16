package main

import (
	"fmt"
	"os"
	"strings"

	"github.com/rancher/rio/cli/pkg/clicontext"
	"github.com/rancher/rio/cli/pkg/stack"
	"github.com/rancher/rio/cli/pkg/types"
	riov1 "github.com/rancher/rio/pkg/apis/rio.cattle.io/v1"
	"github.com/rancher/wrangler/pkg/kubeconfig"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	ns := "default"
	name := os.Args[1]

	loader := kubeconfig.GetInteractiveClientConfig(nil)
	restConfig, err := loader.ClientConfig()
	if err != nil {
		return err
	}
	rio := riov1.NewForConfig(restConfig)

	obj, err := rio.Stacks(ns).Get(name, metav1.GetOptions{})
	if err != nil {
		return err
	}
	s := obj.(*riov1.Stack)
	fmt.Println(s)

	return nil
}
