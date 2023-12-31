# SPDX-FileCopyrightText: 2023 Keitaro Nakamura
# SPDX-License-Identifier: BSD-3-Clause
from setuptools import setup
import os
from glob import glob

package_name = 'ros2robosys2023'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share',package_name), glob('launch/*.launch.xml'))
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='Keitaro Nakamura',
    maintainer_email='keitaro.nakamura@hoge.com',
    description='a package to practice',
    license='BSD-3-Clause',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'talker = ros2robosys2023.talker:main',
            'listener = ros2robosys2023.listener:main',
        ],
    },
)
