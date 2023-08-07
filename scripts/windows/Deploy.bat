set ROOT_REPO_DIR=%cd%
set SOURCES_DIR=%ROOT_REPO_DIR%\src
set DOCKER_SCRIPTS_DIR=%ROOT_REPO_DIR%\scripts\docker
set ARTIFACTS_DIR=%ROOT_REPO_DIR%\.artifacts
set WEBROOT_DIR=%ROOT_REPO_DIR%\webroot
set DOCKER_IMAGE_NAME=web_server_img

docker image build "%DOCKER_SCRIPTS_DIR%" ^
-t %DOCKER_IMAGE_NAME%

docker container run --rm ^
--mount src="%SOURCES_DIR%",target="/sources",type=bind ^
--mount src="%DOCKER_SCRIPTS_DIR%",target="/scripts",type=bind ^
--mount src="%ARTIFACTS_DIR%",target="/artifacts",type=bind ^
-t %DOCKER_IMAGE_NAME% sh -c ./scripts/build.sh

docker container run --rm -d ^
--mount src="%DOCKER_SCRIPTS_DIR%",target="/scripts",type=bind ^
--mount src="%ARTIFACTS_DIR%",target="/artifacts",type=bind ^
--mount src="%WEBROOT_DIR%",target="/webroot",type=bind ^
-p 80:80 ^
-t %DOCKER_IMAGE_NAME% sh -c /scripts/run.sh