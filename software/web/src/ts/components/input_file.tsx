/* esp32-firmware
 * Copyright (C) 2022 Erik Fleckstein <erik@tinkerforge.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

import * as util from "../../ts/util";

import { h, Context, Fragment } from "preact";
import {useContext, useState} from "preact/hooks";
import { JSXInternal } from "preact/src/jsx";
import { Button } from "react-bootstrap";

interface InputFileProps extends Omit<JSXInternal.HTMLAttributes<HTMLInputElement>,  "class" | "id" | "type" | "onInput"> {
    idContext?: Context<string>
    onUploadStart: (f: File) => Promise<boolean>,
    onUploadSuccess: () => void,
    onUploadError: (error: string | XMLHttpRequest) => void,
    browse: string
    select_file: string
    upload: string,
    url: string,
    contentType?: string
    timeout_ms?: number
}

export function InputFile(props: InputFileProps) {
    const [file, setFile] = useState<File>(null);
    const [uploading, setUploading] = useState(false);
    const [progress, setProgress] = useState(0);

    const percent = (progress * 100).toFixed(0);

    const id = useContext(props.idContext);

    const upload = async () => {
        if(!await props.onUploadStart(file))
            return;

        setProgress(0);
        setUploading(true);

        util.upload(file, props.url, setProgress, props.contentType, props.timeout_ms)
            .then(() => {
                setUploading(false);
                setProgress(0);

                props.onUploadSuccess();
            })
            .catch((xhr: XMLHttpRequest) => {
                setUploading(false);
                setProgress(0);

                props.onUploadError(xhr);
            });
    }

    return (<>
        <div class="input-group" hidden={uploading}>
            <div class="custom-file">
                <input type="file" class="custom-file-input form-control"
                    onChange={(ev) => setFile((ev.target as HTMLInputElement).files[0])}
                    id={id}/>
                <label class="custom-file-label form-control rounded-left"
                    for={id} data-browse={props.browse}>{file ? file.name : props.select_file}</label>
            </div>
            <div class="input-group-append">
                <Button className="form-control" variant="primary" onClick={upload} disabled={file == null}>{props.upload}</Button>
            </div>
        </div>
        <div id="firmware-progress" hidden={!uploading}>
            <div class="form-progress mb-1">
                <div id="firmware-progress-bar" class="progress-bar form-control progress-bar-no-transition"
                    role="progressbar" style={"width: " + percent + "%"} aria-valuenow={percent} aria-valuemin="0"
                    aria-valuemax="100"></div>
            </div>
            <label for='firmware-progress-bar' data-i18n="firmware_update.content.uploading"></label>
        </div>
        </>
    );
}