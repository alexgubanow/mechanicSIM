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
            InitializeComponent();
            var vm = new MainViewModel();
            this.DataContext = vm;
            for (int i = 1; i < vm.MainWin.linearModel.timeMoments[0].Points.Length; i += 2)
            {
                var lg = new LineGraph();
                lines.Children.Add(lg);
                lg.Stroke = new SolidColorBrush(Color.FromRgb(255, 0, 0));
                lg.Description = String.Format("Node {0}", i + 1);
                lg.StrokeThickness = 1;
                double[] iojhno = new double[vm.MainWin.linearModel.timeMoments.Length];
                for (int j = 0; j < vm.MainWin.linearModel.timeMoments.Length; j++)
                {
                    iojhno[j] = (vm.MainWin.linearModel.timeMoments[j].Points[i].derivatives.displ[0] + i * 0.0001);
                }
                lg.Plot(vm.MainWin.linearModel.time, iojhno);
            }
        }

        //long _elastic = 215 * pow(10, 9);
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