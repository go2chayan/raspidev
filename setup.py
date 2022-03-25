import setuptools
import re
import subprocess
from io import open
from distutils.command.build import build as _build


# This class handles the pip install mechanism.
class build(_build):
  sub_commands = _build.sub_commands + [('CustomCommands', None)]


CUSTOM_COMMANDS = [
['sudo', 'apt-get', 'update'],
['sudo', 'apt-get', 'upgrade', '-y' ],
['sudo', 'apt-get', 'install', 'rpi.gpio', '-y'],
['python', '-m', 'venv', '~/env']
]


class CustomCommands(setuptools.Command):
  """A setuptools Command class able to run arbitrary commands."""
  def initialize_options(self):
    pass

  def finalize_options(self):
    pass

  def RunCustomCommand(self, command_list):
    print('Running command: %s' % command_list)
    p = subprocess.Popen(
        command_list,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT)
    # Can use communicate(input='y\n'.encode()) if the command run requires
    # some confirmation.
    stdout_data, _ = p.communicate()
    print('Command output: %s' % stdout_data)
    if p.returncode != 0:
      raise RuntimeError(
          'Command %s failed: exit code: %s' % (command_list, p.returncode))

  def run(self):
    for command in CUSTOM_COMMANDS:
      self.RunCustomCommand(command)


def get_version():
    """
    Extract the version from the module's root __init__.py file
    """
    root_init_file = open("raspi_dev/__init__.py").read()
    match = re.search("__version__[ ]+=[ ]+[\"'](.+)[\"']", root_init_file)
    return match.group(1) if match is not None else "unknown"


setuptools.setup(
    name="raspidev",
    version=get_version(),
    author="Md Iftekhar Tanveer",
    author_email="go2chayan@gmail.com",
    description="""
    RaspberryPi Developer (raspidev) is a compendium of installing common
    developer features for raspberry pi. It supports basic python develop
    environment (conda etc.) in raspberry pi.
    """,
    long_description=open("README", "r", encoding='utf-8').read(),
    long_description_content_type="text/markdown",

    packages=setuptools.find_packages(),

    package_data={},

    python_requires='>=3.7',

    install_requires=[
        "pyserial",
    ],

    cmdclass={
        # Command class instantiated and run during pip install scenarios.
        "build": build,
        "CustomCommands": CustomCommands,
    },
)
