//using myMatch;
using InteractiveDataDisplay.WPF;
using linearElem.ViewModel;
using System;
using System.Windows;
using System.Windows.Data;
using System.Windows.Media;

namespace linearElem
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow
    {
        public MainViewModel vm;

        public MainWindow()
        {
            var vm = new MainViewModel();
            InitializeComponent();
            DataContext = vm;
        }
    }

    public class VisibilityToCheckedConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return ((Visibility)value) == Visibility.Visible;
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return ((bool)value) ? Visibility.Visible : Visibility.Collapsed;
        }
    }
    
}