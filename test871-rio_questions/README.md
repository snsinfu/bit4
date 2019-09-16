Updating answers to Rio app questions. The help message says:

```
To update answer file for a stack, run `rio stack update --answers $FILE_PATH $NAME`.
To update images for services in stack, run `rio stack update --images service-foo=nginx $NAME`
```

but `--images` option needs to be specified even when updating`only answers. When
I update answers I do not want to change images, so the `--images` option should
be set to the currently used image.

The `update_answers` script obtains images used by the services in a stack using
`rio stack info` command and updates the stack with a new answers and the same
images.

It works! The example app (myapp) uses `PORT` question for customizing listening
port. Upon answer change, the pod (or pods when scaled up) is immediately
restarted and the app gets up with the new configuration. It's almost like
`heroku config` UX.
