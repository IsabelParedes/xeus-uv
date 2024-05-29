############################################################################
# Copyright (c) 2024, Isabel Paredes                                       #
# Copyright (c) 2024, QuantStack                                           #
#                                                                          #
# Distributed under the terms of the BSD 3-Clause License.                 #
#                                                                          #
# The full license is in the file LICENSE, distributed with this software. #
############################################################################

import unittest
import jupyter_kernel_test


class XeusKernelTests(jupyter_kernel_test.KernelTests):

    kernel_name = "test_kernel_shell"
    language_name = "cpp"

    code_hello_world = "hello, world"
    code_page_something = "?"

    code_execute_result = [
        {'code': '6*7', 'result': '6*7'},
        {'code': 'test', 'result': 'test'}
    ]

    completion_samples = [
        {'text': 'a.', 'matches': ['a.test1', 'a.test2']}
    ]

    complete_code_samples = ["complete"]
    incomplete_code_samples = ["incomplete"]
    invalid_code_samples = ["invalid"]

    code_inspect_sample = "invalid"

    def test_xeus_stderr(self):
        reply, output_msgs = self.execute_helper(code='error')
        self.assertEqual(output_msgs[0]['msg_type'], 'stream')
        self.assertEqual(output_msgs[0]['content']['name'], 'stderr')
        self.assertEqual(output_msgs[0]['content']['text'], 'error')

class XeusIopubWelcomeTests(jupyter_kernel_test.IopubWelcomeTests):

    kernel_name = "test_kernel_shell"
    support_iopub_welcome = True

if __name__ == '__main__':
    unittest.main()
