using linearElem.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using myMatch;
using MahApps.Metro.Controls;
using InteractiveDataDisplay.WPF;

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
            InitializeComponent();
            var vm = new MainViewModel();
            this.DataContext = vm;
            for (int i = 1; i < vm.MainWin.linearModel.timeMoments[0].Nodes.Length; i++)
            {
                var lg = new LineGraph();
                lines.Children.Add(lg);
                lg.Stroke = new SolidColorBrush(Color.FromRgb(255, 0, 0));
                lg.Description = String.Format("Node {0}", i + 1);
                lg.StrokeThickness = 1;
                double[] iojhno = new double[vm.MainWin.linearModel.timeMoments.Length];
                for (int j = 0; j < vm.MainWin.linearModel.timeMoments.Length; j++)
                {
                    iojhno[i] = (vm.MainWin.linearModel.timeMoments[j].Nodes[i].derivatives.displ[0]);
                }
                lg.Plot(vm.MainWin.linearModel.time, iojhno);
            }
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
