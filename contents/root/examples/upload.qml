/* Copyright (c) 2012 Silk Project.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Silk nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL SILK BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

import Silk.HTTP 1.1
import Silk.HTML 5.0
import "./components"

Html {
    UserInput {
        id: input

        property string action

        onSubmit: {
            switch(input.action) {
            case 'upload':
                for (var i = 0; i < http.files.length; i++) {
                    result.text = '%1(%2) uploaded to %3'.arg(http.files[i].fileName).arg(http.files[i].contentType).arg(http.files[i].filePath);
                    if (http.files[i].remove()) {
                        result.text += ' and removed safely.';
                    } else {
                        result.text += '. please remove the file manually.';
                    }
                }
                break;
            default:
                break;
            }
        }
    }

    Head {
        Title { id: title; text: "File upload" }
    }

    Body {
        H1 { text: title.text }
        P {
            id: result
            enabled: text.length > 0
        }

        Form {
            action: http.path
            method: 'POST'
            enctype: "multipart/form-data"
            Input { type: 'file'; name: 'file' }
            Input { type: 'submit'; value: 'Upload' }
            Input { type: 'hidden'; name: 'action'; value: 'upload' }
        }
    }
}
