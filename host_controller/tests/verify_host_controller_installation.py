import pytest
import os
import json


class TestToysAutomationHostControllerinstallation(object):

    def test_toyautomation_dir_exists(self, host):
        assert host.file('/opt/toyautomation/').is_directory

    def test_library_dir_exists(self, host):
        assert host.file('/opt/toyautomation/library').is_directory

    def test_demo_projects_dir_exists(self, host):
        assert host.file('/opt/toyautomation/demo_projects').is_directory

    @pytest.fixture
    def virtualenv_bin_path(self) -> str:
        return '/opt/ve_toyautomation/bin/'

    def test_virtualenv_for_toyautomation_exists(self, host, virtualenv_bin_path: str):
        assert os.path.isabs(virtualenv_bin_path)
        assert host.file(virtualenv_bin_path).is_directory

    def test_virtualenv_for_toyautomation_python_installed(self, host, virtualenv_bin_path: str):
        assert 'Python 3.' in host.check_output(os.path.join(virtualenv_bin_path, "python") + " --version")

    def test_virtualenv_for_ansible_no_outdated_pip_packages(self, host, virtualenv_bin_path):
        outdated_package_list = json.loads(host.check_output(os.path.join(virtualenv_bin_path, "pip") + " list --outdated --format=json"))
        assert len(outdated_package_list) == 0

    def test_sixpair_installed(self, host):
        """
        sixpair is the tool to connect PS3 controllers.

        :param host:
        :return:
        """
        command_output = host.run("/opt/sixpair/sixpair")
        assert 'No controller found on USB busses.' == command_output.stdout.strip()
        assert command_output.failed

