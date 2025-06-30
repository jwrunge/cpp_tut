script_folder="/Users/jacobrunge/Code/cpp_tut/build/Release/generators"
echo "echo Restoring environment" > "$script_folder/deactivate_conanbuildenv-release-armv8.sh"
for v in PATH
do
    is_defined="true"
    value=$(printenv $v) || is_defined="" || true
    if [ -n "$value" ] || [ -n "$is_defined" ]
    then
        echo export "$v='$value'" >> "$script_folder/deactivate_conanbuildenv-release-armv8.sh"
    else
        echo unset $v >> "$script_folder/deactivate_conanbuildenv-release-armv8.sh"
    fi
done


export PATH="/Users/jacobrunge/.conan2/p/cmake3abd806e43ff1/p/CMake.app/Contents/bin:$PATH"