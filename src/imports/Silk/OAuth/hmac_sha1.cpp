/* Copyright (c) 2012 Twitter4QML Project.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Twitter4QML nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL TWITTER4QML BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "hmac_sha1.h"
#include <QCryptographicHash>

static const int BLOCK_SIZE = 64;

QByteArray hmac_sha1(const QByteArray& in_key, const QByteArray& text)
{
    QByteArray key(in_key);
    if (key.length() > BLOCK_SIZE) {
        key = QCryptographicHash::hash(key, QCryptographicHash::Sha1);
    }
    if (key.length() < BLOCK_SIZE) {
        key += QByteArray(BLOCK_SIZE-key.length(), 0);
    }
    QByteArray o_key_pad(BLOCK_SIZE, char(0x5c));
    QByteArray i_key_pad(BLOCK_SIZE, char(0x36));
    for (int i=0; i<BLOCK_SIZE; ++i) {
        o_key_pad[i] = o_key_pad[i] ^ key.at(i);
        i_key_pad[i] = i_key_pad[i] ^ key.at(i);
    }
    QByteArray ret = QCryptographicHash::hash(i_key_pad + text, QCryptographicHash::Sha1);
    ret = QCryptographicHash::hash(o_key_pad + ret, QCryptographicHash::Sha1);
    return ret;
}
