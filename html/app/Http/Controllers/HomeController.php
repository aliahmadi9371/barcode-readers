<?php

namespace App\Http\Controllers;

use App\User;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Session;

class HomeController extends Controller
{
    /**
     * Create a new controller instance.
     *
     * @return void
     */
    public function __construct()
    {
        $this->middleware('auth');
    }

    public function homePage()
    {
        return view('pages.homePage');
    }
    public function storeNewPassword(Request $request)
    {
        $users = new User();
        $user = $users->currentUser();
        $presentPassword = $request['presentPassword'];

        if (password_verify($presentPassword, $user->password)) {
            $user->update(array(
                'password' => bcrypt($request['newPassword']),
            ));

            return Redirect('/')->with(Session::flash('flash_message', 'رمز عبور جدید با موفقیت تغییر یافت.'));

        } else {
            return Redirect('/')->with(Session::flash('flash_d_message', 'رمز عبور وارد شده با رمز عبور فعلی مطابقت ندارد.'));

        }

    }

    public function networkPage(){
        $dns_nameservers = file_get_contents(public_path("/networkSetting/dns_nameservers.txt"));
        $ipAddress = file_get_contents(public_path("/networkSetting/ipAddress.txt"));
        $subnetMask = file_get_contents(public_path("/networkSetting/subnetMask.txt"));
        $gateWay = file_get_contents(public_path("/networkSetting/gateWay.txt"));
        return view('pages.networkSettingPage', compact('dns_nameservers', 'ipAddress', 'subnetMask', 'gateWay'));
    }


    public function changeNetworkSetting(Request $request)
    {




        $netFile = public_path("/networkSetting/interfaces");
        $newNet = '
auto lo
iface lo inet loopback

auto eth0
iface eth0 inet static
    address '.$request['ipAddress'].'
    netmask '.$request['subnetMask'].'
    gateway '.$request['gateWay'].'
    dns-nameservers '.$request['dns_nameservers'];
        file_put_contents($netFile, $newNet);


        $dnsFile = public_path("/networkSetting/dns_nameservers.txt");
        $newDns = $request['dns_nameservers'];
        file_put_contents($dnsFile, $newDns);

        $gatewayFile = public_path("/networkSetting/gateWay.txt");
        $newGateway = $request['gateWay'];
        file_put_contents($gatewayFile, $newGateway);

        $subnetFile = public_path("/networkSetting/subnetMask.txt");


        $newSubnet = $request['subnetMask'];
        file_put_contents($subnetFile, $newSubnet);

        $ipFile = public_path("/networkSetting/ipAddress.txt");
        $newIp = $request['ipAddress'];
        file_put_contents($ipFile, $newIp);
        exec(public_path("/networkSetting/net.sh"));

        return Redirect('/network-setting')->with(Session::flash('flash_message', 'ﺎﻃﻼﻋﺎﺗ ﺶﺑکﻩ ﺏﺍ ﺪﻣﻮﻔﻗیﺕ ﻭیﺭﺍیﺵ ﺵﺩ.'));

    }

    public function serverPage(){
        $serverTimeout = file_get_contents(public_path("/serverSetting/serverTimeout.txt"));
        $entrance = file_get_contents(public_path("/serverSetting/entrance.txt"));
        $exit = file_get_contents(public_path("/serverSetting/exit.txt"));
        $password = file_get_contents(public_path("/serverSetting/password.txt"));
        $username = file_get_contents(public_path("/serverSetting/username.txt"));
        $deviceId = file_get_contents(public_path("/serverSetting/deviceId.txt"));
        $relaytimeout = file_get_contents(public_path("/serverSetting/relaytimeout.txt"));
        $soapUrl = file_get_contents(public_path("/serverSetting/soapurl.txt"));
        return view('pages.serverSettingPage', compact( 'exit', 'serverTimeout', 'entrance', 'password', 'username', 'deviceId', 'relaytimeout', 'soapUrl'));
    }

    public function reset(Request $request)
    {
        exec(public_path("/networkSetting/reboot.sh"));
        exit();
    }


    public function changeServerSetting(Request $request)
    {
        $netFile = public_path("/networkSetting/config.ini");


        $newNet = '[BarcodeHw]
usb6="/dev/input/by-path/platform-sunxi-ohci.2-usb-0:1:1.0-event-kbd"
usb7="/dev/input/by-path/platform-sunxi-ohci.3-usb-0:1:1.0-event-kbd"
usb8="/dev/input/by-path/platform-sunxi-ohci.4-usb-0:1:1.0-event-kbd"
[IO]
enterance='.$request['enterance'].'
exit='.$request['exit'].'
[Server]
soapurl="'.$request['soapUrl'].'"
[Info]
id='.$request['deviceId'].'
user="'.$request['username'].'"
pass="'.$request['password'].'"
[SoftWare]
tReplyIn='.$request['serverTimeout'].'
tReplyOut='.$request['serverTimeout'].'
tGateOpen='.$request['relaytimeout'].'
debug=false
[number]
readBarcode=0


';


/*

        $newNet = '[server]
srv='.$request['serverName'].'
port='.$request['port'].'
baseurl='.$request['baseurl'].'
[urlinfo]
gatetype='.$request['gateType'].'
password='.$request['password'].'
username='.$request['username'].'
deviceId='.$request['deviceId'].'
relaytimeout='.$request['relaytimeout'].'

';
*/


        file_put_contents($netFile, $newNet);




        $serverTimeoutFile = public_path("/serverSetting/serverTimeout.txt");
        $newServerTimeout = $request['serverTimeout'];
        file_put_contents($serverTimeoutFile, $newServerTimeout);


        $passwordFile = public_path("/serverSetting/password.txt");
        $newPassword = $request['password'];
        file_put_contents($passwordFile, $newPassword);

        $usernameFile = public_path("/serverSetting/username.txt");
        $newUsername = $request['username'];
        file_put_contents($usernameFile, $newUsername);

        $deviceIdFile = public_path("/serverSetting/deviceId.txt");
        $newDeviceId = $request['deviceId'];
        file_put_contents($deviceIdFile, $newDeviceId);

        $relaytimeoutFile = public_path("/serverSetting/relaytimeout.txt");
        $newRelaytimeout = $request['relaytimeout'];
        file_put_contents($relaytimeoutFile, $newRelaytimeout);

        $entranceFile = public_path("/serverSetting/entrance.txt");
        $newEntranceFileFile = $request['enterance'];
        file_put_contents($entranceFile, $newEntranceFileFile);

        $exitFile = public_path("/serverSetting/exit.txt");
        $newExit = $request['exit'];
        file_put_contents($exitFile, $newExit);

        $soapUrlFile = public_path("/serverSetting/soapurl.txt");
        $newSoapUrlFile = $request['soapUrl'];
        file_put_contents($soapUrlFile, $newSoapUrlFile);

        exec(public_path("/networkSetting/conf.sh"));
        return Redirect('/server-setting')->with(Session::flash('flash_message', 'اطلاعات سرور با موفقیت ویرایش شد.'));

    }


}
