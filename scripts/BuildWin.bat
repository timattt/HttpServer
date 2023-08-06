set ROOT_REPO_DIR=%cd%
set SOURCES_DIR=%ROOT_REPO_DIR%\src
set SCRIPTS_DIR=%ROOT_REPO_DIR%\scripts
set ARTIFACTS_DIR=%ROOT_REPO_DIR%\.build

set DOCKER_IMAGE_NAME=test_img

docker image build "%SCRIPTS_DIR%" ^
-t %DOCKER_IMAGE_NAME%

docker container run --rm ^
--mount src="%SOURCES_DIR%",target="/sources",type=bind ^
--mount src="%SCRIPTS_DIR%",target="/scripts",type=bind ^
--mount src="%ARTIFACTS_DIR%",target="/artifacts",type=bind ^
-t %DOCKER_IMAGE_NAME% sh -c ./scripts/build.sh

docker container run --rm ^
--mount src="%SCRIPTS_DIR%",target="/scripts",type=bind ^
--mount src="%ARTIFACTS_DIR%",target="/artifacts",type=bind ^
-p 80:80 ^
-t %DOCKER_IMAGE_NAME% sh -c /scripts/run.sh

REM docker image rm -f %DOCKER_IMAGE_NAME%