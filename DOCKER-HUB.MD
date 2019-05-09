# Automated Builds on Docker Hub

To build these demo's automatically with Docker Hub, we can do the following:

 1. Login to the [Docker Hub](https://hub.docker.com)
 2. (We'll assume you're a member of the ARPA2 team, otherwise ask)
 3. Click `Repositories` in the top bar, and the `arpa2` project
 4. Click `Create Repository` on the right side
 5. Enter, for `arpa2`, a name such as `build-pip` and a brief description
 6. Make it publicly visible
 7. For Build Settings, connect to GitHub
 8. For the project connection, select `arpa2` and `docker-demo`
 9. Customise, possibly by entering and editing later so that:
     - Tag with Source `/^build-pip-latest$/` receives Docker Tag `latest` for Dockerfile `/demo-pip/Dockerfile` or actually just `Dockerfile` relative to `/demo-pip` but that doesn't show up initially.
     - Tag with Source `/^build-pip-([0-9.]+)$/` receives Docker Tag `{\1}` for Dockerfile `/demo-pip/Dockerfile` or actually just `Dockerfile` relative to `/demo-pip` but that doesn't show up initially.
10. Click `Create` on the bottom, go to the top for `Builds` and right side for `Configure Automatic Builds` and split the Dockerfile Location `/demo-pip/Dockerfile` into `Dockerfile` and Build Context `/demo-pip` which is the reference for files named in the Dockerfile.  This is not possible in the first go, or so it seems.
11. Now close with `Save and Build`

After this, we can go to the repository, and test build the example `build-bin`.  When satisfied, we can notify Docker Hub with tags:

```
docker tag --force build-bin-latest
docker push --tags --force
```
