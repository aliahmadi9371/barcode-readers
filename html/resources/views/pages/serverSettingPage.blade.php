@extends('layout.admin')


@section('title')
    <title>تنظیمات سرور</title>
@stop
<style>
    input{
        text-align: left;
    }
    label, button{
        float: left;
    }

    @media (min-width: 768px) {
        .setting {
            float: left !important;
        }

    }

</style>

@section('content')
    @if (Session::has('flash_message'))
        <div class="alert alert-info" style="background-color: #00FF00">{{ Session::get('flash_message') }}</div>
    @endif
    <div style=" padding-bottom: 10%;" class="container-fluid ">


        <div class="wrapper wrapper-content animated fadeInRight">

            <div class="row">
                <div class="col-md-8 col-md-offset-2 col-sm-8 col-sm-offset-2">
                    <div class="ibox float-e-margins">
                        <div class="ibox-title">
                            <h5 style="float: left;">Application Setting</h5>
                        </div>
                        <div class="ibox-content">
                            <div class="row">
                                <div style="padding-top: 3%;" class="setting col-sm-12 ">
                                    <form role="form" id="form" method="POST" action="/server-setting" class="form-horizontal">
                                        {{ csrf_field() }}
                                        <input type="hidden" name="_token" value="{{ csrf_token() }}">

                                        <div class="form-group"><label for="subnetMask"
                                                                       class="setting col-sm-3 control-label">Web Service Url
                                            </label>
                                            <div class="setting col-sm-8">
                                                <input class="form-control" type="text" id="soapUrl"
                                                       name="soapUrl" value="{{$soapUrl}}"
                                                >
                                            </div>
                                        </div>
                                        <div class="form-group"><label for="gateWay"
                                                                       class="setting col-sm-3 control-label">Entrance Barcode Reader
                                            </label>
                                            <div class="setting col-sm-8">
                                                <input class="form-control" type="text" name="enterance" id="enterance"
                                                       value="{{$entrance}}">
                                            </div>
                                        </div>
    <div class="form-group"><label for="gateWay"
                                                                       class="setting col-sm-3 control-label">Exit Barcode Reader
                                            </label>
                                            <div class="setting col-sm-8">
                                                <input class="form-control" type="text" name="exit" id="exit"
                                                       value="{{$exit}}">
                                            </div>
                                        </div>

                                        <div class="form-group"><label for="dns"
                                                                       class="setting col-sm-3 control-label">Device Id
                                            </label>
                                            <div class="setting col-sm-8">
                                                <input class="form-control" type="text" name="deviceId" id="deviceId"
                                                       value="{{ $deviceId }}">
                                            </div>
                                        </div>


                                        <div class="form-group"><label for="dns"
                                                                       class="setting col-sm-3 control-label">UserName
                                            </label>
                                            <div class="setting col-sm-8">
                                                <input class="form-control" type="text" name="username" id="username"
                                                       value="{{ $username }}">
                                            </div>
                                        </div>

                                        <div class="form-group"><label style=" float: left;" for="dns"
                                                                       class="setting col-sm-3 control-label">Password
                                            </label>
                                            <div class="setting col-sm-8">
                                                <input class="form-control" type="text" name="password" id="password"
                                                       value="{{ $password }}">
                                            </div>
                                        </div>

                                        <div class="form-group"><label for="dns"
                                                                       class="setting col-sm-3 control-label"> (Relay Timeout (ms
                                            </label>
                                            <div class="setting col-sm-8">
                                                <input class="form-control" type="text" name="relaytimeout" id="relaytimeout"
                                                       value="{{ $relaytimeout }}">
                                            </div>
                                        </div>

                                        <div class="form-group"><label for="dns"
                                                                       class="setting col-sm-3 control-label"> (Server Timeout (ms
                                            </label>
                                            <div class="setting col-sm-8">
                                                <input class="form-control" type="text" name="serverTimeout" id="serverTimeout"
                                                       value="{{ $serverTimeout }}">
                                            </div>
                                        </div>

                                        <div class="form-group"><label
                                                                       class="setting col-sm-3 control-label">
                                            </label>
                                            <div class="setting col-sm-8">
                                                <button class="btn btn-primary" type="submit">Save
                                                </button>
                                            </div>
                                        </div>
                                    </form>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>

@stop


@section('scripts')

    <script>


        $(document).ready(function () {

            $("#form").validate({
                rules: {
                    soapUrl: {
                        required: true,

                    },
                    enterance: {
                        required: true,

                    },
                    exit: {
                        required: true,

                    },
                    serverTimeout: {
                        required: true,

                    },
                    username: {
                        required: true,

                    },
                    deviceId: {
                        required: true,

                    },
                    relaytimeout: {
                        required: true,

                    },

                }

            });

        });

    </script>





@stop