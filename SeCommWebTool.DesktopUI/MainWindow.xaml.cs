using System;
using System.Collections.Generic;
using System.Configuration;
using System.Diagnostics;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Forms;
using System.Windows.Interop;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Threading;


namespace SeCommWebTool.DesktopUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private static string pathTillNamespace = Directory.GetParent(Directory.GetParent(System.IO.Directory.GetCurrentDirectory()).ToString()).ToString();
        private List<DeviceInfo> connectedDevices;
        private NotifyIcon ni;
        private ContextMenuStrip menu;
        
        private ToolStripMenuItem submenu, item;
        private Dictionary<int, string> imageMap;
        private Dictionary<string, string> languageMap;
        private  void MapLanguages()
        {
            languageMap = new Dictionary<string, string>();
            languageMap.Add("English","en-US");
            languageMap.Add("Danish","da-DK");
        }
        private void SetRequirements()
        {
            MapLanguages();
            MapImages();
            SetTray();
            
        }
        private Configuration GetConfigObject()
        {
            string appPath = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            string configFile = System.IO.Path.Combine(appPath, "App.config");
            ExeConfigurationFileMap configFileMap = new ExeConfigurationFileMap();
            configFileMap.ExeConfigFilename = configFile;
            Configuration config = ConfigurationManager.OpenMappedExeConfiguration(configFileMap, ConfigurationUserLevel.None);
            return config;
        }
        [DllImport("AgentApp.dll", EntryPoint = "InitializeAgent", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        public static extern void InitializeAgent();
        [DllImport("AgentApp.dll", EntryPoint = "GetEvents", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetEvents(IntPtr wParam, IntPtr lParam);
        [DllImport("AgentApp.dll", EntryPoint = "ExposeDatatoUI", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr ExposeDatatoUI(ref int noOfDevices);


        [StructLayout(LayoutKind.Sequential)]
        public class DeviceInfo
        {
            [MarshalAsAttribute(UnmanagedType.LPStr)]
            public string deviceName;
            [MarshalAsAttribute(UnmanagedType.LPStr)]
            public string manufacturer;
            [MarshalAsAttribute(UnmanagedType.LPStr)]
            public string deviceId;
            public int productId;
            [MarshalAsAttribute(UnmanagedType.LPStr)]
            public string firmwareVersion;
            [MarshalAsAttribute(UnmanagedType.LPStr)]
            public string serialNo;
            [MarshalAsAttribute(UnmanagedType.I1)]
            public bool isDefault;
        }
        private void MapImages()
        {
            imageMap = new Dictionary<int, string>();
            imageMap.Add(0, "NoHeadsetSelected.png");
            imageMap.Add(45, "Sennheiser BTD 800 USB for Lync.png");
            imageMap.Add(46, "Sennheiser Presence.png");

        }
        public MainWindow()
        {
            InitializeComponent();
            //GetProxy();
            IEnumerable<string> paths = new string[] { pathTillNamespace + @"\Dlls\" };
            AddEnvironmentPaths(paths);            
            RightPane.Visibility = Visibility.Hidden;
            SetRequirements();
            this.ShowInTaskbar = false;
            InitializeAgent();
            //StartTimer(1);
        }
        private void GetProxy()
        {
            WebProxy proxy = (WebProxy)WebRequest.DefaultWebProxy;
            if (proxy.Address.AbsoluteUri != string.Empty)
            {
                
            }
        }
        private void StartTimer(int hours)
        {
            System.Windows.Threading.DispatcherTimer dispatcherTimer = new System.Windows.Threading.DispatcherTimer();
            dispatcherTimer.Tick += new EventHandler(dispatcherTimer_Tick);
            dispatcherTimer.Interval = new TimeSpan(hours, 0, 0);
            dispatcherTimer.Start();
        }
        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            // call your startApp method 
        }
        public static void AddEnvironmentPaths(IEnumerable<string> paths)
        {
            var path = new[] { Environment.GetEnvironmentVariable("PATH") ?? string.Empty };
            string newPath = string.Join(System.IO.Path.PathSeparator.ToString(), path.Concat(paths));
            Environment.SetEnvironmentVariable("PATH", newPath);
        }

        protected override void OnSourceInitialized(EventArgs e)
        {
            base.OnSourceInitialized(e);
            HwndSource source = PresentationSource.FromVisual(this) as HwndSource;
            if (source != null)
            {
                source.AddHook(WndProc);
                UsbNotification.RegisterUsbDeviceNotification(source.Handle);
            }
        }

        private IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            DeviceInfo ms;
            if (msg == 537)
            {
                GetEvents(wParam, lParam);
                int noOfDevices = 0;
                int structSize = Marshal.SizeOf(typeof(DeviceInfo));
                IntPtr myStructs = ExposeDatatoUI(ref noOfDevices);
                if (noOfDevices == 0)
                {
                    RightPane.Visibility = System.Windows.Visibility.Hidden;
                    DevicesComboBox.Items.Clear();
                    DevicesComboBox.Items.Refresh();
                    ImagePanel.Source = new BitmapImage(new Uri(pathTillNamespace + @"\Image\" + imageMap[0]));
                    Home_SetAsDefault.IsChecked = false;
                    return IntPtr.Zero;
                }
                connectedDevices = new List<DeviceInfo>();
                for (int i = 0; i < noOfDevices; ++i)
                {
                    IntPtr data = new IntPtr(myStructs.ToInt64() + structSize * i);
                    ms = (DeviceInfo)Marshal.PtrToStructure(data, typeof(DeviceInfo));
                    if (ms.deviceId == null) return IntPtr.Zero;
                    connectedDevices.Add(ms);
                }
                AddDevicesToDropdown(connectedDevices);
                SetUI(connectedDevices.First());
            }

            return IntPtr.Zero;
        }

        private void AddDevicesToDropdown(List<DeviceInfo> connectedDevices)
        {
            DevicesComboBox.Items.Clear();
            foreach (var i in connectedDevices)
            {
                DevicesComboBox.Items.Add(i.deviceName);
            }
            DevicesComboBox.Items.Refresh();
        }

        private void SetUI(DeviceInfo device)
        {
            if (device.isDefault) Home_SetAsDefault.IsChecked = true;
            else Home_SetAsDefault.IsChecked = false;
            Home_DeviceIDLabel.Content = device.deviceId;
            Home_DeviceNameLabel.Content = device.deviceName;
            Home_FirmwareVersionLabel.Content = device.firmwareVersion;
            Home_ManufacturerLabel.Content = device.manufacturer;
            Home_ProductIDLabel.Content = "0x" + device.productId.ToString("X");
            Home_SerialNumberLabel.Content = device.serialNo;
            ImagePanel.Source = new BitmapImage(new Uri(pathTillNamespace + @"\Image\" + imageMap[device.productId]));
            DevicesComboBox.Text = device.deviceName;
            RightPane.Visibility = System.Windows.Visibility.Visible;

        }

        private void frmMain_Resize(object sender, EventArgs e)
        {

            if (WindowState.Minimized == this.WindowState)
            {
                ni.BalloonTipText = "Running in background";
                ni.BalloonTipTitle = "HeadSet UI";
                ni.ShowBalloonTip(1);
            }

        }

        private void SetTray()
        {
            ni = new NotifyIcon();
            ni.Icon = new Icon(pathTillNamespace + @"\Image\Tray.ico");
            ni.Visible = true;
            menu = new ContextMenuStrip();

            submenu = new ToolStripMenuItem();
            submenu.Text = "About...";
            submenu.Click += new EventHandler(About_Click);
            menu.Items.Add(submenu);

            submenu = new ToolStripMenuItem();
            submenu.Text = "Open...";
            submenu.Click += new EventHandler(Open_Click);
            menu.Items.Add(submenu);

            submenu = new ToolStripMenuItem();
            submenu.Text = "Help...";
            submenu.Click += new EventHandler(Help_Click);
            menu.Items.Add(submenu);

            submenu = new ToolStripMenuItem();
            submenu.Text = "Language";
            submenu.Name = "Language";
            Configuration config = GetConfigObject();
            string langPref=config.AppSettings.Settings["LanguagePreference"].Value;            
            CultureResources.ChangeCulture(new CultureInfo(languageMap[langPref]));
            foreach(var x in languageMap)
            {
                item = new ToolStripMenuItem();
                item.Name = x.Key;
                item.Text = x.Key;
                if (langPref == x.Key)
                {
                    item.Checked = true;
                }
                item.Click += new EventHandler(Language_Click);
                submenu.DropDownItems.Add(item);
            }            
            menu.Items.Add(submenu);
            submenu = new ToolStripMenuItem();
            submenu.Text = "Exit";
            submenu.Click += new EventHandler(Exit_Click);
            menu.Items.Add(submenu);
            ni.ContextMenuStrip = menu;
        }

        protected void Open_Click(Object sender, EventArgs e)
        {

            System.Windows.Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Background,
                new Action(delegate()
                {
                    this.WindowState = WindowState.Normal;
                    this.Activate();
                })
            );
            this.Show();
            this.ShowInTaskbar = true;
        }

        protected void About_Click(Object sender, EventArgs e)
        {
            Hide();
        }

        protected void Language_Click(Object sender, EventArgs e)
        {
            UncheckRemainingLanguages();
            var clickedMenuItem = sender as ToolStripMenuItem;
            clickedMenuItem.Checked = true;
            CultureResources.ChangeCulture(new CultureInfo(languageMap[clickedMenuItem.Name]));
            LanguageUpdateInConfig(clickedMenuItem.Name);
        }      
        
        private void LanguageUpdateInConfig(string language)
        {

            Configuration config = GetConfigObject();
            config.AppSettings.Settings.Remove("LanguagePreference");
            config.AppSettings.Settings.Add("LanguagePreference", language);
            config.Save(ConfigurationSaveMode.Modified);
             
        }
        private void UncheckRemainingLanguages()
        {
            foreach (ToolStripMenuItem x in menu.Items)
            {
                if (x.Name == "Language")
                {
                    foreach (ToolStripMenuItem y in x.DropDownItems)
                    {
                        if (y.Checked) y.Checked = false;
                    }
                }
            }
        }
        protected void Help_Click(Object sender, EventArgs e)
        {

        }

        protected void Exit_Click(Object sender, EventArgs e)
        {
            ni.Visible = false;
            System.Windows.Application.Current.Shutdown();
        }

        private void CloseButton_Click(object sender, RoutedEventArgs e)
        {
            WindowState = WindowState.Minimized;
            this.ShowInTaskbar = false;
            frmMain_Resize(sender, e);
        }

        private void MoveWindow(object sender, DragDeltaEventArgs e)
        {
            Left = Left + e.HorizontalChange;
            Top = Top + e.VerticalChange;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            WindowState = WindowState.Minimized;
            this.ShowInTaskbar = false;
            frmMain_Resize(sender, e);
        }

        private void Hyperlink_RequestNavigate(object sender, RequestNavigateEventArgs e)
        {
            Process.Start(new ProcessStartInfo(e.Uri.AbsoluteUri));
            e.Handled = true;
        }

        private void DevicesComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            foreach (var i in connectedDevices)
            {
                if (i.deviceName.Equals(DevicesComboBox.SelectedValue)) SetUI(i);
            }

        }

    }
}
