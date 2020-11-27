import os
import sys

if __name__ == "__main__":
    # Structure of data in coverage_stats
    #
    # coverage_stats=
    # {filename: 
    #     {
    #         'lines_executed': int
    #         'tines_total': int
    #         'reported_percent': int
    #     }
    # }
    coverage_stats = {}

    # Load the names of all source files
    for filename in os.listdir(os.path.join(os.pardir, 'fontdiff')):
        if filename.endswith('.cc') or filename.endswith('h'):
            coverage_stats[filename] = {
                'lines_executed': 0,
                'lines_total': 0,
                'reported_percent': 0
            }

    # read data from the log
    with open(os.path.join(os.pardir, os.pardir, 'coverage', 'coverage.log'), 'r') as log:
        while True:
            file_info = log.readline()
            file_data = log.readline()
            if not file_data or not file_info: break    #EOF

            # some files cannot be analyzed/were not executed; skip them
            if file_data.startswith('No'): continue

            # parse the strings
            filename = file_info.split('/')[-1][0:-2]
            
            tmp = file_data.split(':')[1].split()
            pct = float(tmp[0].strip('%'))
            lines = int(tmp[-1])

            if filename in coverage_stats:
                coverage_stats[filename]['reported_percent'] = pct
                coverage_stats[filename]['lines_total'] = lines
                coverage_stats[filename]['lines_executed'] = int(pct * lines / 100)

    # accumulate the stats for the overall project
    overall_stats = {'lines_executed': 0,
                     'lines_total': 0 }

    for val in coverage_stats.values():
        for k, v in val.items():
            if k == 'reported_percent': continue
            overall_stats[k] += v

    # write the json file
    output = 'coverage_report.json' if len(sys.argv) == 1 else sys.argv[1]
    with open(os.path.join(os.pardir, os.pardir, output), 'w') as report:
        report.write('{\n')
        # section for overall stats
        report.write('\t"GLOBAL": {\n')
        report.write('\t\t"percent_coverage": '
                + str(overall_stats['lines_executed'] / float(overall_stats['lines_total']) * 100)
                + ",\n")
        report.write('\t\t"lines_executed": ' + str(overall_stats['lines_executed']) + ',\n')
        report.write('\t\t"total_lines": ' + str(overall_stats['lines_total']) + ',\n')
        report.write('\t},\n')

        report.write('\t"FILES:" {\n')
        
        # write each file's data
        for filename, data in coverage_stats.items():
            report.write('\t\t"' + filename + '": {\n')

            for k, v in data.items():
                report.write('\t\t\t"' + k + '": ' + str(v) + ",\n")

            report.write('\t\t"},\n')
        report.write('\t}\n')
        report.write('}')

