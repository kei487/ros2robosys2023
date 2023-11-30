from setuptools import find_packages, setup
import os
from glob import glob

package_name = 'mkpkg'

setup(
    name=package_name,
    version='0.0.1',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
	(os.path.join('share',package_name),glob('launch/*.launch.py'))
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='Keitaro Nakamura',
    maintainer_email='keitaro@hogehoge.com',
    description='a package for practice',
    license='BSD-3-Clause',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'talker = mkpkg.talker:main',
            'listener = mkpkg.listener:main',
        ],
    },
)
